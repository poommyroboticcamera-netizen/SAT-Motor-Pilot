"""Migrate presentation strings; keep source notices and compatibility identifiers."""
from pathlib import Path
import re,json
root=Path(__file__).resolve().parents[1]
source=root/'source'
token=re.compile(r'/\*[\s\S]*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"')
records=[]
for p in source.rglob('*'):
    if not p.is_file() or 'reference' in p.relative_to(source).parts: continue
    if p.suffix not in {'.cpp','.qml','.ui'}: continue
    # Transport bytes, schema and serializer sources are outside this migration.
    if p.name in {'commands.cpp','packet.cpp','vbytearray.cpp','configparams.cpp','configparam.cpp'}: continue
    old=p.read_text(encoding='utf-8')
    def convert(text, generic=False):
        text=text.replace('VESC® Tool','SAT Motor Pilot').replace('VESC Tool','SAT Motor Pilot').replace('MotorLab','SAT Motor Pilot')
        if generic:
            text=text.replace('VESC Packages','Controller Packages').replace('VESC Remote','Remote Control')
            text=re.sub(r'\bVESCs\b','controllers',text)
            text=re.sub(r'\bVESC(?:®)?-based\b','compatible',text)
            # Keep explicit hardware model identities accurate.
            text=text.replace('VESC Express','__REAL_HW_EXPRESS__')
            text=re.sub(r'\bVESC(?:®)?(?=\W|$)','controller',text)
            text=text.replace('__REAL_HW_EXPRESS__','VESC Express')
        return text
    if p.suffix=='.ui':
        new=re.sub(r'(<string(?: [^>]*)?>)([\s\S]*?)(</string>)',lambda m:m[1]+convert(m[2],True)+m[3],old)
    else:
        def replace(m):
            t=m[0]
            if not t.startswith('"') or 'github.com/' in t or 'vesc-project.com' in t or 'GPL-' in t or 'Modified build based' in t: return t
            # QObject APIs, namespaces, command names and raw file paths stay unchanged.
            if p.name=='main.cpp' and ('setOrganization' in old[max(0,m.start()-60):m.start()] or 'MotorLab.Desktop' in t): return t
            return convert(t, p.suffix=='.qml' and ' ' in t and not t.startswith('"qrc:') and '://' not in t)
        new=token.sub(replace,old)
    if new!=old:
        p.write_text(new,encoding='utf-8',newline='\n')
        records.append(str(p.relative_to(root)))
(root/'evidence/product-text-migration.json').write_text(json.dumps({'scope':'Quoted product presentation strings only; source notices, packets, schema keys and values excluded','files':records},indent=2),encoding='utf-8')
print('Updated product presentation in',len(records),'files')
