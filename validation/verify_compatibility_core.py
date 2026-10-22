"""Verify the product conversion preserved the previously tested compatibility core."""
from pathlib import Path
import subprocess,hashlib,json,sys
root=Path(__file__).resolve().parents[1]
baseline='f6631534ad00c48b01fc1124a832ccf573d41d86'
paths=['source/'+n for n in ['commands.cpp','commands.h','packet.cpp','packet.h','vbytearray.cpp','vbytearray.h','configparams.cpp','configparams.h','configparam.cpp','configparam.h','datatypes.h']]
paths += [p.relative_to(root).as_posix() for p in (root/'source/res/config').rglob('*.xml')]
records=[]
for path in paths:
    previous=subprocess.check_output(['git','-C',str(root),'show',baseline+':'+path]).replace(b'\r\n',b'\n')
    current=(root/path).read_bytes().replace(b'\r\n',b'\n')
    records.append({'path':path,'identical':previous==current,'sha256':hashlib.sha256(current).hexdigest()})
# These exact functions carry bytes between the commands and each transport.
def body(data,name):
    start=data.index('void VescInterface::'+name+'(')
    opening=data.index('{',start); depth=1; pos=opening+1
    while depth:
        depth += (data[pos]=='{')-(data[pos]=='}');pos+=1
    return data[start:pos].replace('\r\n','\n')
path='source/vescinterface.cpp'
before=subprocess.check_output(['git','-C',str(root),'show',baseline+':'+path]).decode('utf-8')
after=(root/path).read_text(encoding='utf-8')
for name in ['packetDataToSend','serialDataAvailable','tcpInputDataAvailable','udpInputDataAvailable']:
    left,right=body(before,name),body(after,name)
    # Only the audit diagnostic product name changed in packetDataToSend.
    left=left.replace('"MotorLab audit: physical packet transmission blocked"', '"SAT Motor Pilot audit: physical packet transmission blocked"')
    records.append({'function':name,'identical':left==right,'sha256':hashlib.sha256(right.encode()).hexdigest()})
passed=all(r['identical'] for r in records)
output={'baseline':baseline,'purpose':'Product conversion regression: source text identity after CRLF normalization; transport bodies allow only the audit diagnostic product-name replacement. Does not establish live hardware compatibility.','checks':len(records),'pass':passed,'records':records}
(root/'evidence/company-core-regression.json').write_text(json.dumps(output,indent=2),encoding='utf-8')
print(f'Compatibility core unchanged: {sum(r["identical"] for r in records)}/{len(records)}')
sys.exit(0 if passed else 1)
