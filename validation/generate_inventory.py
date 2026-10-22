from pathlib import Path
import csv,re,xml.etree.ElementTree as ET,json
root=Path(__file__).resolve().parents[1]; src=root/'source'; out=root/'validation'
rows=[]
def add(kind,p,line,name,text='',acceptance='Open and exercise in Windows runtime; compare screenshot, enabled state and result against stable 7.00 reference'):
 rows.append(dict(id=f'UI-{len(rows)+1:05}',kind=kind,source=p.relative_to(src).as_posix(),line=line,name=name,label=text,evidence='source-observed',runtime='PENDING',acceptance=acceptance))
files=[]
for p in sorted(src.rglob('*')):
 if 'reference' in p.relative_to(src).parts: continue
 if p.suffix not in ('.ui','.qml'): continue
 text=p.read_text(encoding='utf-8',errors='replace'); files.append(p.relative_to(src).as_posix())
 add('form' if p.suffix=='.ui' else 'qml-component',p,1,p.stem)
 if p.suffix=='.ui':
  tree=ET.fromstring(text)
  for el in tree.iter():
   if el.tag not in ('widget','action','item'): continue
   name=el.get('name',''); cls=el.get('class',el.tag)
   props=[]
   for prop in el.findall('property'):
    if prop.get('name') in ('text','title','windowTitle','toolTip'):
     s=prop.find('string')
     if s is not None and s.text: props.append(s.text)
   if name or props:
    line=next((i for i,l in enumerate(text.splitlines(),1) if f'name="{name}"' in l),1) if name else 1
    add('ui-'+cls,p,line,name,' | '.join(props))
 else:
  for i,l in enumerate(text.splitlines(),1):
   m=re.search(r'\b(text|title|placeholderText|objectName)\s*:\s*(.*)',l)
   if m: add('qml-property',p,i,m[1],m[2].strip())
# Inventory all directly implemented function entry points in application source, excluding vendored libraries.
cpps=[src/'mainwindow.cpp',src/'preferences.cpp',src/'parametereditor.cpp',src/'boardsetupwindow.cpp']
cpps+=list((src/'pages').glob('*.cpp'))+list((src/'widgets').glob('*.cpp'))+list(src.glob('*wizard*.cpp'))
for p in sorted(set(cpps)):
 if not p.exists(): continue
 for i,l in enumerate(p.read_text(encoding='utf-8',errors='replace').splitlines(),1):
  m=re.match(r'^(?:[\w:<>,*&]+\s+)?([A-Za-z_]\w*::[~A-Za-z_]\w*)\s*\((.*)',l)
  if m:add('cpp-entry',p,i,m[1],l.strip(),'Invoke owning page/control and verify result, failure behavior and hardware requirements; source existence is not execution proof')
  if re.search(r'addPageItem\(|setTitle\(|setSubTitle\(|setPage\(',l):add('navigation-or-wizard',p,i,l.strip())
with (out/'ui-function-traceability.csv').open('w',newline='',encoding='utf-8-sig') as f:
 w=csv.DictWriter(f,fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)
(out/'inventory-summary.json').write_text(json.dumps({'source_commit':'01d5f10901116c311e3fb84d5a1541f663d3ce20','scope':'Every .ui and .qml file recursively; C++ entry points in pages/widgets/wizards/main window/preferences/parameter and board editors. Vendored UI included and identified by source path. Dynamic parameter definitions require separate XML inventory.','forms_and_qml':len(files),'traceability_rows':len(rows),'runtime_pass_count':0,'files':files},indent=2),encoding='utf-8')
print(len(files),'forms/QML;',len(rows),'traceability rows')
