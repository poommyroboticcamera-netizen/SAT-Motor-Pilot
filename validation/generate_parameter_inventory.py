from pathlib import Path
import csv,xml.etree.ElementTree as E,re
p=Path(__file__).resolve().parents[1];s=p/'source';o=p/'validation'
rows=[]
for f in sorted((s/'res/config').glob('**/parameters_*.xml')):
 for e in E.parse(f).getroot().findall('./Params/*'):
  rows.append({'source':f.relative_to(s).as_posix(),'firmware':f.parent.name,'parameter':e.tag,'label':e.findtext('longName',''),'type':e.findtext('type',''),'default_int':e.findtext('valInt',''),'default_double':e.findtext('valDouble',''),'minimum':e.findtext('minDouble',e.findtext('minInt','')),'maximum':e.findtext('maxDouble',e.findtext('maxInt','')),'enum_options':' | '.join(x.text or '' for x in e.findall('enumNames')),'runtime':'PENDING'})
with (o/'parameter-traceability.csv').open('w',newline='',encoding='utf-8-sig') as f:
 w=csv.DictWriter(f,fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)
commands=[];value=-1
for i,line in enumerate((s/'datatypes.h').read_text().splitlines(),1):
 m=re.match(r'\s*(COMM_\w+)\s*(?:=\s*(\d+))?\s*[,}]',line)
 if m:
  value=int(m[2]) if m[2] else value+1
  commands.append({'command':m[1],'id':value,'source':'datatypes.h','line':i,'runtime':'PENDING','acceptance':'Verify request bytes, timeout, response decode and firmware compatibility against recorded reference exchange'})
with (o/'command-traceability.csv').open('w',newline='',encoding='utf-8-sig') as f:
 w=csv.DictWriter(f,fieldnames=list(commands[0]));w.writeheader();w.writerows(commands)
print(len(rows),'parameter records;',len(commands),'command enum records')

