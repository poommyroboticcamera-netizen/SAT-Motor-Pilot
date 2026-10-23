from pathlib import Path
import json,csv
root=Path(__file__).resolve().parents[1]
audit=json.loads((root/'evidence/ui/motorlab/runtime-audit.json').read_text(encoding='utf-8'))
rows=[]
def add(feature,implementation,test,result,evidence,remaining):
    rows.append(dict(feature=feature,implementation=implementation,test=test,result=result,evidence=evidence,remaining=remaining))
for n,item in enumerate(audit.get('workspaceNavigation',[]),1):
    add('SAT commissioning navigation '+str(n),'source/pages/pagewelcome.cpp; source/mainwindow.cpp','Click actual '+item['button']+' and inspect current stacked page','PASS' if item['pass'] else 'FAIL','evidence/ui/motorlab/runtime-audit.json#workspaceNavigation','Connected workflow and hardware acceptance')
add('Company application identity','source/main.cpp; source/sat_motor_pilot.pro','Read QApplication organization/name/domain and Windows executable metadata','PASS' if audit.get('applicationName')=='SAT Motor Pilot' and audit.get('organizationName')=='SomboonAdvanceTechnology' else 'FAIL','evidence/ui/motorlab/runtime-audit.json','Other hosts/install profiles')
manual=audit.get('manualPanel',{})
add('Manual drive drawer','source/satshell.h','Click real toggle twice; inspect visibility and count emitted commands','PASS' if all(manual.get(k) for k in ['exists','collapsedByDefault','expands','collapses']) and manual.get('commandsSentByToggle')==0 else 'FAIL','evidence/ui/motorlab/runtime-audit.json#manualPanel','Physical drive commands remain untested')
add('Windows dark title bar','source/satshell.h','Check native DWM attribute application succeeded on this Windows host','PASS' if audit.get('darkTitlebarApplied') else 'FAIL','evidence/ui/motorlab/runtime-audit.json#darkTitlebarApplied','Other Windows versions and system themes')
for item in audit.get('onboardingPages',[]):
    expectedBlocked=item['id'] in [1,2]
    passed=item.get('screenshotSaved',False) and (not expectedBlocked or not item['completeBeforeAcceptance'])
    add('Company onboarding '+str(item['id']),'source/satproduct.h; source/startupwizard.cpp','Render page and inspect mandatory acceptance state','PASS' if passed else 'FAIL','evidence/ui/motorlab/'+item['screenshot'],'Full manual completion/keyboard/focus/DPI coverage')
add('Company help/build/update ownership','source/mainwindow.cpp; source/utility.cpp; source/res/sat_changelog.md','Inspect source: no startup checkVersion calls; checkVersion contains no network request; own GitHub links','SOURCE_REVIEWED','docs/COMPANY_PRODUCT.md','Exercise every help action and external link')
for item in audit['configurationRoundtrips']:
    add('Local '+item['kind']+' XML persistence','source/configparams.cpp; source/motorlabaudit.h','Perturb destination, save/load XML, compare all parameters and serialized bytes','PASS' if item['pass'] else 'FAIL','evidence/ui/motorlab/runtime-audit.json#configurationRoundtrips','Physical controller read/write')
with (root/'validation/company-product-traceability.csv').open('w',newline='',encoding='utf-8-sig') as f:
    w=csv.DictWriter(f,fieldnames=rows[0].keys());w.writeheader();w.writerows(rows)
result={'product':'SAT Motor Pilot 0.3.0','checks':len(rows),'runtimePassed':sum(r['result']=='PASS' for r in rows),'failed':sum(r['result']=='FAIL' for r in rows),'sourceReviewed':sum(r['result']=='SOURCE_REVIEWED' for r in rows),'scope':'Company UI/navigation/onboarding/local files; hardware not tested','rows':rows}
(root/'evidence/company-product-acceptance.json').write_text(json.dumps(result,indent=2,ensure_ascii=False),encoding='utf-8')
print('Company acceptance:',result['runtimePassed'],'runtime checks,',result['failed'],'failed,',result['sourceReviewed'],'source review')
if result['failed']: raise SystemExit(1)
