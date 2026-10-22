"""Compare actual audit PNGs without rescaling or suppressing branding differences."""
from pathlib import Path
from PIL import Image, ImageChops, ImageDraw
import json

root = Path(__file__).resolve().parents[1]
base = root / 'evidence/ui'
out = base / 'comparison'
out.mkdir(exist_ok=True)
reports = []
reference = json.loads((base/'reference/runtime-audit.json').read_text(encoding='utf-8'))
modified = json.loads((base/'motorlab/runtime-audit.json').read_text(encoding='utf-8'))
ref_pages = {p['row']: p for p in reference['pages']}
for page in modified['pages']:
    row = page['row']
    match = ref_pages.get(row)
    if not match:
        reports.append({'row':row, 'comparable':False, 'reason':'Missing reference row'})
        continue
    a = Image.open(base/'reference'/match['screenshot']).convert('RGB')
    b = Image.open(base/'motorlab'/page['screenshot']).convert('RGB')
    record = {'row':row, 'title':page['title'], 'referenceSize':list(a.size), 'motorlabSize':list(b.size), 'comparable':a.size == b.size}
    if a.size == b.size:
        delta = ImageChops.difference(a,b)
        changed = sum(1 for p in delta.getdata() if max(p) > 8)
        record.update(changedPixelsAbove8=changed, fractionChanged=changed/(a.width*a.height), exact=not delta.getbbox())
        delta.save(out/f'diff-{row:03}.png')
        canvas = Image.new('RGB', (a.width*2, a.height+36), '#151b26')
        canvas.paste(a,(0,36)); canvas.paste(b,(a.width,36))
        draw = ImageDraw.Draw(canvas)
        draw.text((16,10),'Pinned upstream source reference (instrumented)',fill='white')
        draw.text((a.width+16,10),'MotorLab modified source build',fill='white')
        canvas.save(out/f'side-by-side-{row:03}.png')
    reports.append(record)
(out/'metrics.json').write_text(json.dumps({'scope':'Disconnected native Windows source builds; all differences retained, including intentional branding. Pixel metrics are not functional equivalence.', 'pages':reports},indent=2,ensure_ascii=False),encoding='utf-8')
print(f'Compared {len(reports)} page pairs; size matches: {sum(p["comparable"] for p in reports)}')
