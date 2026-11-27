"""Original procedural 512px materials and restored winter workwear atlas. Offline/reproducible."""
from pathlib import Path
from PIL import Image, ImageDraw, ImageFilter, ImageFont
import numpy as np,random,re,math,glob
R=Path(__file__).resolve().parents[1];A=R/'art/district190';A.mkdir(exist_ok=True)
rng=random.Random(190);nr=np.random.default_rng(190);N=512

def noise(base):
 a=np.zeros((N,N),dtype=float)
 for f,amp in [(5,8),(17,5),(69,2.2)]:
  low=Image.fromarray(nr.integers(0,256,(f,f),dtype=np.uint8)).resize((N,N),Image.Resampling.BICUBIC)
  a+=(np.asarray(low,dtype=float)-128)*amp/64
 a+=nr.normal(0,1.7,(N,N))
 return Image.fromarray(np.clip(np.array(base)[None,None,:]+a[:,:,None],0,255).astype('uint8'))
def material(name,kind):
 im=noise({'concrete':(151,155,147),'wallpaper':(188,184,163),'wood':(146,123,91),'parquet':(151,129,93),'tiles':(167,181,166),'metal':(152,158,145),'brick':(150,147,126)}[kind]);d=ImageDraw.Draw(im)
 if kind=='concrete':
  for k in range(4200):
   x,y=rng.randrange(N),rng.randrange(N);c=rng.randrange(112,171);d.ellipse((x,y,x+rng.randrange(1,3),y+rng.randrange(1,3)),fill=(c,c+4,c))
  for k in range(3):
   x,y=rng.randrange(N),rng.randrange(N);pts=[(x,y)]
   for t in range(17):x+=rng.randint(-9,9);y+=rng.randint(3,12);pts.append((x,y))
   d.line(pts,fill=(114,123,115),width=1)
  d.line((1,0,1,N),fill=(117,125,117),width=2);d.line((0,1,N,1),fill=(124,131,123),width=2)
 elif kind=='wallpaper':
  for y in range(24,N,64):
   for x in range(24+(y//64%2)*32,N,64):
    for dx,dy in [(0,-7),(7,0),(0,7),(-7,0)]:d.ellipse((x+dx-3,y+dy-5,x+dx+3,y+dy+5),outline=(162,164,141),width=1)
    d.line((x,y+6,x+2,y+18),fill=(166,167,145))
  d.line((0,0,0,N),fill=(160,163,144),width=2)
 elif kind in ('wood','parquet'):
  if kind=='parquet':
   for y in range(0,N,32):
    for x in range(-128,N,128):
     xx=x+(y//32%2)*64;col=(rng.randrange(131,163),rng.randrange(110,134),rng.randrange(77,103));d.rectangle((xx,y,xx+127,y+31),fill=col);d.line((xx,y,xx+127,y),fill=(104,98,73));d.line((xx,y,xx,y+31),fill=(109,99,73));
     for k in range(8):d.line((xx+4,y+3+k*3,xx+124,y+4+k*3),fill=tuple(c+rng.randrange(-7,8) for c in col))
  else:
   for x in range(N):
    offset=math.sin(x*.11)*5+math.sin(x*.031)*4
    d.line([(x+int(math.sin(y*.017+x*.01)*3),y) for y in range(0,N,8)],fill=(int(139+offset),int(116+offset),int(84+offset)),width=1)
   for x,y in [(115,140),(370,390)]:
    for r in range(2,25,3):d.ellipse((x-r//3,y-r,x+r//3,y+r),outline=(121,105,79))
 elif kind=='tiles':
  for k in range(0,N,64):d.line((k,0,k,N),fill=(117,133,123),width=3);d.line((0,k,N,k),fill=(117,133,123),width=3);d.line((k+3,0,k+3,N),fill=(193,201,181))
 elif kind=='metal':
  for k in range(2300):
   x,y=rng.randrange(N),rng.randrange(N);r=rng.choice([1,1,1,2,4]);d.ellipse((x,y,x+r,y+r),fill=(128,112,86) if k%5==0 else (138,148,134))
  for x in range(0,N,64):d.line((x,0,x,N),fill=(130,142,130),width=2)
 elif kind=='brick':
  for y in range(0,N,40):
   d.line((0,y,N,y),fill=(104,114,104),width=3)
   for x in range(-40,N,80):
    xx=x+(y//40%2)*40;d.line((xx,y,xx,y+40),fill=(104,114,104),width=3)
 im.save(A/(name+'.png'));return im

def header(name,im,repeat=1):
 pix=[];w,h=im.size
 while True:
  arr=np.asarray(im,dtype=np.uint32);p=(arr[:,:,0]<<16)|(arr[:,:,1]<<8)|arr[:,:,2];pix.extend(map(int,p.ravel()))
  if im.width==im.height==1:break
  im=im.resize((max(1,im.width//2),max(1,im.height//2)),Image.Resampling.BOX)
 return 'static const U32 '+name+'_pixels['+str(len(pix))+']={'+','.join(hex(v) for v in pix)+'};\nstatic const Texture '+name+'={0,'+str(w)+','+str(h)+','+str(repeat)+','+name+'_pixels};\n'
room='/* Original v190 512px materials, including complete mip chains. */\n'
for name in ['wallpaper','wood','tiles','parquet']:room+=header('art_'+name,material(name,name))
(R/'src/room_art.h').write_text(room)
(R/'src/postpunk_materials.h').write_text(header('art_concrete170',material('concrete','concrete')))
extra=header('art_metal190',material('metal','metal'))+header('art_brick190',material('brick','brick'))
fontpath=glob.glob('/usr/share/fonts/**/arialbd.ttf',recursive=True)
if not fontpath:fontpath=glob.glob('/usr/share/fonts/**/NotoSans-Bold.ttf',recursive=True)
font=ImageFont.truetype(fontpath[0],52) if fontpath else ImageFont.truetype('DejaVuSans-Bold.ttf',52)
for i,(label,color) in enumerate([('АПТЕКА',(72,114,93)),('ПОЧТА',(71,104,111)),('ПАРИКМАХЕРСКАЯ',(104,100,86))]):
 im=Image.new('RGB',(512,128),color);d=ImageDraw.Draw(im);d.rectangle((3,3,508,124),outline=(168,178,151),width=2)
 f=font
 while d.textbbox((0,0),label,font=f)[2]>470:f=ImageFont.truetype(font.path,f.size-2)
 b=d.textbbox((0,0),label,font=f);d.text(((512-b[2])/2,(128-(b[3]-b[1]))/2-b[1]),label,font=f,fill=(202,204,173));im.save(A/('sign_'+str(i)+'.png'));extra+=header('sign190_'+str(i),im.resize((512,512),Image.Resampling.BICUBIC),0)
extra+='static const Texture sign190[3]={{0,512,512,0,sign190_0_pixels},{0,512,512,0,sign190_1_pixels},{0,512,512,0,sign190_2_pixels}};\n'
(R/'src/materials190.h').write_text(extra)
# Preserve face/skin/boot cells; re-author the four actual coat variants, with woven fibres,
# quilt seams, zip tape, cuff wear and deliberately restrained colour.
im=Image.open(R/'art/characters/civilian_atlas.png').convert('RGB');d=ImageDraw.Draw(im)
for v,base in enumerate([(102,112,95),(103,92,112),(111,100,81),(82,100,108)]):
 for y in range(128):
  for x in range(128):
   fold=int(5*math.sin(x*.12+math.sin(y*.03)));fibre=(x+y*3)%3-1;seam=-18 if y%19 in (0,1) else 0
   zip=-14 if x in (62,63,64) else 0;c=tuple(max(0,min(255,k+fold+fibre+seam+zip)) for k in base);im.putpixel((x,y+v*128),c)
 for y in range(7,120,4):d.line((58,y+v*128,60,y+v*128),fill=(147,148,125))
im.save(A/'winter_workwear_atlas.png');im.save(R/'art/characters/civilian_atlas.png')
s=(R/'src/civilian_mesh.h').read_text();pixheader=header('rig190',im,0);arr=re.search(r'static const U32 rig190_pixels\[\d+\]=\{.*?\};',pixheader).group(0).replace('rig190_pixels','rigPixels');s=re.sub(r'static const U32 rigPixels\[\d+\]=\{.*?\};',lambda m:arr,s);(R/'src/civilian_mesh.h').write_text(s)
(A/'PROVENANCE.txt').write_text('Original offline procedural art authored for this project. Generator: tools/build_art190.py. No downloaded game assets or supplied reference images embedded. Workwear atlas preserves original project face/skin cells; clothing cells newly authored.\n')
print('Wrote eight 512px environmental/winter materials, workwear atlas and three Cyrillic signs.')

# Keep the editable GLB's embedded clothing in sync with the runtime atlas.
import json,struct
p=R/'art/characters/civilian_rig.glb'
b=p.read_bytes();jl,jt=struct.unpack_from('<II',b,12);gltf=json.loads(b[20:20+jl]);bl,bt=struct.unpack_from('<II',b,20+jl);buf=bytearray(b[28+jl:28+jl+bl]);vi=gltf['images'][0]['bufferView'];v=gltf['bufferViews'][vi]
if gltf['asset'].get('extras',{}).get('clothing190'):buf=buf[:v['byteOffset']]
while len(buf)%4:buf.append(0)
png=(R/'art/characters/civilian_atlas.png').read_bytes();v['byteOffset']=len(buf);v['byteLength']=len(png);buf.extend(png)
while len(buf)%4:buf.append(0)
gltf['buffers'][0]['byteLength']=len(buf);gltf['asset'].setdefault('extras',{})['clothing190']=True
j=json.dumps(gltf,separators=(',',':')).encode();j+=b' '*((-len(j))%4)
p.write_bytes(struct.pack('<III',0x46546c67,2,28+len(j)+len(buf))+struct.pack('<II',len(j),jt)+j+struct.pack('<II',len(buf),bt)+buf)
print('Updated editable GLB: original skinning, new embedded winter clothing.')
