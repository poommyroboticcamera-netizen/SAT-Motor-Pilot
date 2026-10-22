"""Package the user-supplied company artwork without redrawing its mark."""
from pathlib import Path
from PIL import Image
import shutil
root = Path(__file__).resolve().parents[1]
assets = root/'source/res/motorlab'
original = assets/'somboon-user-supplied.png'
image = Image.open(original).convert('RGBA')
# Retain the supplied artwork intact. Only scale/pad for application resource slots.
for relative in ['logo.png','logo_light.png']:
    canvas = Image.new('RGBA',(480,144),'white')
    scaled = image.copy(); scaled.thumbnail((144,144),Image.Resampling.LANCZOS)
    canvas.alpha_composite(scaled,(0,0))
    from PIL import ImageDraw,ImageFont
    draw=ImageDraw.Draw(canvas)
    font=ImageFont.truetype('C:/Windows/Fonts/arialbd.ttf',48)
    draw.text((155,42),'MotorLab',font=font,fill='#304b8a')
    canvas.save(assets/relative)
for relative in ['res/logo_vertical.png','res/+theme_light/logo_vertical.png','res/symbol_v_wide.png','res/+theme_light/symbol_v_wide.png']:
    target=root/'source'/relative
    size=Image.open(target).size
    canvas=Image.new('RGBA',size,'white'); scaled=image.copy(); scaled.thumbnail(size,Image.Resampling.LANCZOS)
    canvas.alpha_composite(scaled,((size[0]-scaled.width)//2,(size[1]-scaled.height)//2)); canvas.save(target)
image.save(assets/'app-icon.png')
image.save(assets/'icon.ico',sizes=[(16,16),(32,32),(48,48),(64,64),(128,128),(256,256)])
print('Company artwork packaged; original retained unchanged')
