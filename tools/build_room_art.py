from pathlib import Path
from PIL import Image
R=Path(__file__).resolve().parents[1];a=R/'art/reference_pass';im=Image.open(a/'materials_source.png').convert('RGB')
h=['/* AI-generated source materials, cropped and downsampled for OKNO v099. */']
for name,rect in [('wallpaper',(0,0,512,512)),('parquet',(512,0,1024,512)),('wood',(0,512,512,1024)),('tiles',(512,512,1024,1024))]:
 p=im.crop(rect).resize((256,256),Image.Resampling.LANCZOS);p.save(a/(name+'.png'));packed=[]
 while True:
  packed.extend((r<<16)|(g<<8)|b for r,g,b in p.getdata())
  if p.width==1:break
  p=p.resize((p.width//2,p.height//2),Image.Resampling.BOX)
 h+=['static const U32 art_'+name+'_pixels[87381]={'+','.join(hex(v) for v in packed)+'};','static const Texture art_'+name+'={0,256,256,1,art_'+name+'_pixels};']
(R/'src/room_art.h').write_text('\n'.join(h).replace('art_wallpaper={0,256,256,1,','art_wallpaper={0,256,256,3,'))
