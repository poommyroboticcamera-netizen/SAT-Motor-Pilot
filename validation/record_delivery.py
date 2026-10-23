from pathlib import Path
import hashlib,json,subprocess
root=Path(__file__).resolve().parents[1]
files=['dist/SATMotorPilot/SATMotorPilot.exe','SAT-Motor-Pilot-Windows-x64.zip','source/packet.cpp','source/vbytearray.cpp','source/commands.cpp','source/configparams.cpp','source/motorlabconfigwrite.h','source/main.cpp','source/satshell.h','source/satstartup.h','source/sattypography.h','source/res/motorlab/somboon-user-supplied.png']
records=[]
for relative in files:
    p=root/relative
    if p.exists(): records.append({'path':relative,'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
audit=json.loads((root/'evidence/ui/motorlab/runtime-audit.json').read_text(encoding='utf-8'))
result={'product':'SAT Motor Pilot 0.3.0 engineering preview','companyArtwork':'User supplied Somboon Advance Technology','upstreamAppCommit':'01d5f10901116c311e3fb84d5a1541f663d3ce20','upstreamFirmwareCommit':'26af69484014b6771d02e68c22f4cb96645b5f22','buildSourceCommit':subprocess.check_output(['git','-C',str(root),'rev-parse','HEAD'],text=True).strip(),'toolchain':'Qt5.15.2 MinGW8.1 Windows x64','protocolTests':{'passed':11,'failed':0,'substantiveMethods':9},'renderedDisconnectedPages':len(audit['pages']),'localXmlChecks':audit['configurationRoundtrips'],'hardwareTesting':'NOT RUN; no controller access or actuation requested','acceptance':'OPEN; see docs/ACCEPTANCE_AND_KNOWN_ISSUES.md','files':records}
(root/'evidence/delivery-manifest.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
print('Delivery manifest recorded')
