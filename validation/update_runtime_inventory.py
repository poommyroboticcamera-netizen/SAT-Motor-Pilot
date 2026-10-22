from pathlib import Path
import csv,json,xml.etree.ElementTree as E
r=Path(__file__).resolve().parents[1];v=r/'validation';s=r/'source';audit=r/'evidence/ui/motorlab/runtime-audit.json'
a=json.loads(audit.read_text(encoding='utf-8-sig')); classes={}
for f in s.rglob('*.ui'):
 if 'reference' in f.relative_to(s).parts:continue
 try:
  c=E.parse(f).getroot().findtext('class');classes[c]=f.relative_to(s).as_posix()
 except E.ParseError:pass
mapping=[]
for p in a['pages']:
 if not p.get('screenshotSaved'):continue
 candidates=[c['class'] for c in p.get('visibleControls',[]) if c.get('class','').startswith('Page')]
 for c in candidates:
  if c in classes:mapping.append({'source':classes[c],'class':c,'runtime_row':p['row'],'title':p.get('title',''),'status':'RENDERED_ONLY','evidence':'evidence/ui/motorlab/'+p['screenshot'],'scope':'Disconnected screenshot only; controls, tabs, dialogs and functions not exercised'})
path=v/'ui-function-traceability.csv';rows=list(csv.DictReader(path.open(encoding='utf-8-sig')))
by_source={m['source']:m for m in mapping}
for row in rows:
 if row['kind']=='form' and row['source'] in by_source:
  m=by_source[row['source']];row['runtime']='RENDERED_ONLY';row['evidence']='source-observed; '+m['evidence'];row['acceptance']='Disconnected page screenshot saved. Controls, tabs, dialogs and functions remain PENDING.'
with path.open('w',newline='',encoding='utf-8-sig') as f:
 w=csv.DictWriter(f,fieldnames=rows[0]);w.writeheader();w.writerows(rows)
with (v/'rendered-page-evidence.csv').open('w',newline='',encoding='utf-8-sig') as f:
 w=csv.DictWriter(f,fieldnames=mapping[0]);w.writeheader();w.writerows(mapping)
print('Screenshot pages',len(a['pages']),'source form rows marked rendered only',len(by_source))

