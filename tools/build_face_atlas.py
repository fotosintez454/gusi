from pathlib import Path
from PIL import Image
R=Path(__file__).resolve().parents[1]
im=Image.open(R/'art/characters/faces16_source.png').convert('RGB').resize((512,512),Image.Resampling.LANCZOS)
im.save(R/'art/characters/faces16.png');pixels=[];a=im
while True:
 pixels.extend((r<<16)|(g<<8)|b for r,g,b in a.getdata())
 if a.width==1:break
 a=a.resize((a.width//2,a.height//2),Image.Resampling.BOX)
(R/'src/faces16.h').write_text('/* Original AI-generated fictional portraits. */\nstatic const U32 facePixels[349525]={'+','.join(hex(v) for v in pixels)+'};\nstatic const Texture faceTexture={0,512,512,0,facePixels};\n')
