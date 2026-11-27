"""Original editable low-poly civilian + 23-joint glTF skin. No downloaded art."""
from pathlib import Path
import math,struct,json
from PIL import Image,ImageDraw
R=Path(__file__).resolve().parents[1]; A=R/'art/characters';A.mkdir(parents=True,exist_ok=True)
names=['pelvis','spine0','spine1','spine2','neck','head','clavicle_R','upperarm_R','forearm_R','hand_R','clavicle_L','upperarm_L','forearm_L','hand_L','thigh_R','shin_R','foot_R','toe_R','thigh_L','shin_L','foot_L','toe_L','root']
parent=[22,0,1,2,3,4,3,6,7,8,3,10,11,12,0,14,15,16,0,18,19,20,-1]
# parents are intentionally root-last in source; runtime traversal uses explicit dependency order.
rest=[(0,.90,0),(0,1.03,0),(0,1.20,0),(0,1.40,0),(0,1.49,0),(0,1.62,0),(.13,1.43,0),(.25,1.38,0),(.53,1.38,0),(.77,1.38,0),(-.13,1.43,0),(-.25,1.38,0),(-.53,1.38,0),(-.77,1.38,0),(.115,.88,0),(.115,.48,0),(.115,.09,.015),(.115,.07,.19),(-.115,.88,0),(-.115,.48,0),(-.115,.09,.015),(-.115,.07,.19),(0,0,0)]
verts=[];faces=[]
# Vertex: global bind position, two joint indices, blend, UV, material.
def v(p,bone,uv=(0,0),mat=0,b2=None,w=1):
 verts.append((p,bone,bone if b2 is None else b2,w,uv,mat));return len(verts)-1
def face(ids,lod=1):faces.append((ids,lod))
shape=[(math.cos(2*math.pi*k/16),math.sin(2*math.pi*k/16)) for k in range(16)]
def rings(rings,bone,mat=0,lod=1):
 start=len(verts)
 for i,(x,y,z,rx,rz) in enumerate(rings):
  for k,(a,b) in enumerate(shape):v((x+a*rx,y,z+b*rz),bone,(k/16,i/max(1,len(rings)-1)),mat)
 for i in range(len(rings)-1):
  for k in range(16):face([start+i*16+k,start+i*16+(k+1)%16,start+(i+1)*16+(k+1)%16,start+(i+1)*16+k],lod)
 for i in [0,len(rings)-1]:
  c=v(rings[i][:3],bone,(.5,.5),mat)
  for k in range(16):face([c,start+i*16+k,start+i*16+(k+1)%16],lod)
def limb(a,b,r0,r1,bone,endbone,mat):
 # Cross section perpendicular to bone axis, two-joint blend only near the seam.
 axis=[b[k]-a[k] for k in range(3)];L=math.sqrt(sum(x*x for x in axis));axis=[x/L for x in axis]
 u=[0,0,1] if abs(axis[2])<.9 else [1,0,0];vv=[axis[1]*u[2]-axis[2]*u[1],axis[2]*u[0]-axis[0]*u[2],axis[0]*u[1]-axis[1]*u[0]]
 start=len(verts)
 for j,t in enumerate([0,.18,.82,1]):
  rr=r0*(1-t)+r1*t
  for k in range(12):
   ang=2*math.pi*k/12;p=tuple(a[d]+axis[d]*L*t+rr*(u[d]*math.cos(ang)+vv[d]*math.sin(ang)) for d in range(3))
   v(p,bone,(k/12,t),mat,endbone,1 if t<.82 else (.94 if t<1 else .55))
 for j in range(3):
  for k in range(12):face([start+j*12+k,start+j*12+(k+1)%12,start+(j+1)*12+(k+1)%12,start+(j+1)*12+k])
def patch(points,bone,mat,lod=0):
 face([v(p,bone,uv,mat) for p,uv in zip(points,[(0,1),(1,1),(1,0),(0,0)])],lod)
rings([(0,.84,0,.195,.135),(0,.92,0,.22,.145),(0,1.04,0,.217,.15),(0,1.23,0,.235,.155),(0,1.36,0,.25,.145),(0,1.43,0,.17,.105)],1)
rings([(0,1.39,0,.13,.13),(0,1.47,0,.12,.115),(0,1.50,0,.07,.075)],4,5)
rings([(0,1.49,0,.067,.075),(0,1.53,.01,.108,.10),(0,1.64,0,.137,.13),(0,1.75,-.005,.124,.111),(0,1.80,-.015,.075,.075)],5,2)
# New shaped hair volume, ears and UV-projected face; no painted-on eye blocks.
rings([(0,1.737,-.015,.140,.122),(0,1.795,-.018,.132,.118),(0,1.829,-.026,.095,.085),(0,1.837,-.025,.035,.035)],5,4)
patch([(-.027,1.595,.128),(.027,1.595,.128),(.020,1.662,.133),(0,1.620,.174)],5,2)
for side in [-1,1]:
 patch([(side*.131,1.592,.005),(side*.153,1.600,.030),(side*.151,1.665,.022),(side*.130,1.676,.004)],5,2)
# Jacket seam, pockets, cuff and trouser construction are part of authored geometry.
patch([(-.008,.92,.157),(.008,.92,.157),(.008,1.39,.173),(-.008,1.39,.173)],1,6)
for s in [-1,1]:
 x=s*.126;patch([(x-.060,.985,.16),(x+.060,.985,.16),(x+.060,1.067,.17),(x-.060,1.067,.17)],1,4)
 rings([(s*.25,1.295,0,.063,.067),(s*.25,1.34,0,.082,.082),(s*.25,1.41,0,.078,.078),(s*.25,1.46,0,.035,.04)],7 if s==1 else 11,0)
 limb(rest[7 if s==1 else 11],rest[8 if s==1 else 12],.093,.071,7 if s==1 else 11,8 if s==1 else 12,0)
 limb(rest[8 if s==1 else 12],rest[9 if s==1 else 13],.072,.046,8 if s==1 else 12,9 if s==1 else 13,0)
 # Palms follow the hand joint, no individual animated fingers in this LOD.
 b=9 if s==1 else 13;x=s*.80
 rings([(x,1.33,0,.044,.05),(x,1.39,0,.05,.055),(x,1.43,0,.039,.045)],b,2)
 t=14 if s==1 else 18;k=t+1;f=t+2
 limb(rest[t],rest[k],.105,.081,t,k,1);limb(rest[k],rest[f],.082,.065,k,f,1)
 rings([(s*.115,.025,.075,.091,.15),(s*.115,.14,.075,.097,.15),(s*.115,.20,.01,.071,.085)],f,3)
# UV split at face boundaries: front gets the whole face, side/back gets skin only.
oldfaces=faces;faces=[]
for ids,lod in oldfaces:
 if all(verts[i][1]==5 and verts[i][5]==2 for i in ids):
  front=sum(verts[i][0][2] for i in ids)/len(ids)>.055
  new=[]
  for i in ids:
   p,b,b2,w,uv,mat=verts[i]
   uv=(max(0,min(1,(p[0]+.137)/.274)),max(0,min(1,(1.80-p[1])/.31))) if front else (.025,max(.3,min(.85,(1.8-p[1])/.31)))
   new.append(v(p,b,uv,12 if front else 9,b2,w))
  face(new,lod)
 else:face(ids,lod)
# Remove unused old face vertices, keep indexed topology for all other body parts.
used=sorted(set(i for ids,lod in faces for i in ids));remap={n:i for i,n in enumerate(used)}
verts=[verts[i] for i in used];faces=[([remap[i] for i in ids],lod) for ids,lod in faces]
for i,(p,b,b2,w,uv,m) in enumerate(verts):
 if b==1 and m==0:
  b2=3;w=max(0,min(1,(1.43-p[1])/.40))
 if m==2 and b!=5:m=9
 if m==4:m=10 if b==5 else 0
 if b==5:
  p=(p[0]*.82,1.49+(p[1]-1.49)*.91,p[2]*.87)
 verts[i]=(p,b,b2,w,uv,m)
# AI-generated original face source; all clothing/skin-side/hair texturing authored here.
im=Image.new('RGB',(512,512),(94,83,66))
colors={0:(80,91,77),1:(123,113,93),2:(172,136,108),3:(58,43,33),5:(115,105,84),6:(41,38,32),7:(190,180,150),9:(171,134,107),10:(50,39,31)}
for mat,base in colors.items():
 ox=(mat%4)*128;oy=(mat//4)*128
 for yy in range(128):
  for xx in range(128):
   noise=((xx*17+yy*29+(xx*yy)%47)%11)-5
   fold=int(7*math.sin(xx*.16+math.sin(yy*.055)*1.4)) if mat in (0,1) else int(5*math.sin(xx*.65+yy*.1)) if mat==10 else 0
   im.putpixel((ox+xx,oy+yy),tuple(max(0,min(255,c+noise+fold)) for c in base))
for variant,base in enumerate([(80,91,77),(91,81,105),(92,83,70),(64,77,88)]):
 for yy in range(128):
  for xx in range(128):
   fold=int(5*math.sin(xx*.08+math.sin(yy*.025)*1.2));noise=((xx//3)*19+(yy//3)*37)%3-1
   seam=-16 if xx%31<2 or yy in (8,9,116,117) else 0
   im.putpixel((xx,variant*128+yy),tuple(max(0,min(255,c+fold+noise+seam)) for c in base))
faceim=Image.open(R/'art/reference_pass/face_source.png').convert('RGB').resize((128,128),Image.Resampling.LANCZOS)
im.paste(faceim,(256,0))
variants=Image.open(R/'art/reference_pass/face_variants_source.png').convert('RGB')
for cell,rect in [(11,(0,0,512,512)),(13,(512,0,1024,512)),(14,(0,512,512,1024)),(15,(512,512,1024,1024))]:im.paste(variants.crop(rect).resize((128,128),Image.Resampling.LANCZOS),((cell%4)*128,(cell//4)*128))
im.save(A/'civilian_atlas.png')
def uv_at(uv,mat):
 if mat==12:return uv
 x=mat%4;y=mat//4
 return ((x*128+3+uv[0]*121)/512,(y*128+3+uv[1]*121)/512)
# Header common to runtime and glTF. Positions are bind-space metres.
h=['/* Generated by tools/build_character.py. Original OKNO civilian, CC0. */','#define RIG_JOINTS 23','typedef struct{float x,y,z,u,v,w;unsigned char b0,b1,mat;} RigVertex;','typedef struct{unsigned short a,b,c;unsigned char lod;} RigFace;']
h+=['static const signed char rigParent[23]={'+','.join(map(str,parent))+'};','static const char*rigNames[23]={'+','.join(json.dumps(n) for n in names)+'};','static const V3 rigRest[23]={'+','.join('{'+','.join(f'{c:.7f}f' for c in p)+'}' for p in rest)+'};']
h+=['static const RigVertex rigVerts[]={']
for p,b,b2,w,uv,m in verts:
 u,vv=uv_at(uv,m);h.append('{'+','.join(f'{q:.7f}f' for q in (*p,u,vv,w))+f',{b},{b2},{m}'+'},')
h.append('};');triangles=[]
for ids,lod in faces:
 for i in range(1,len(ids)-1):triangles.append((ids[0],ids[i],ids[i+1],lod))
h.append('static const RigFace rigFaces[]={'+','.join('{'+','.join(map(str,t))+'}' for t in triangles)+'};')
packed=[];img=im
while True:
 packed.extend((r<<16)|(g<<8)|b for r,g,b in img.getdata())
 if img.width==1:break
 img=img.resize((img.width//2,img.height//2),Image.Resampling.BOX)
h.append('static const U32 rigPixels[349525]={'+','.join(hex(x) for x in packed)+'};')
h.append('static const Texture rigTexture={0,512,512,0,rigPixels};')
(R/'src/civilian_mesh.h').write_text('\n'.join(h))
# Portable GLB 2.0 with POSITION, UV, JOINTS_0, WEIGHTS_0, skin and a walk animation.
buf=bytearray();views=[];acc=[]
def data(raw,component,typ,count,bounds=None):
 while len(buf)%4:buf.append(0)
 off=len(buf);buf.extend(raw);vi=len(views);views.append({'buffer':0,'byteOffset':off,'byteLength':len(raw)})
 a={'bufferView':vi,'componentType':component,'count':count,'type':typ}
 if bounds:a['min'],a['max']=bounds
 acc.append(a);return len(acc)-1
pos=[p for p,*_ in verts];pa=data(struct.pack('<%sf'%(len(pos)*3),*(q for p in pos for q in p)),5126,'VEC3',len(pos),([min(p[k] for p in pos) for k in range(3)],[max(p[k] for p in pos) for k in range(3)]))
uv=data(struct.pack('<%sf'%(len(verts)*2),*(q for p,b,b2,w,t,m in verts for q in uv_at(t,m))),5126,'VEC2',len(verts))
jo=data(bytes(q for p,b,b2,w,t,m in verts for q in [b,b2,0,0]),5121,'VEC4',len(verts))
we=data(struct.pack('<%sf'%(len(verts)*4),*(q for p,b,b2,w,t,m in verts for q in [w,1-w,0,0])),5126,'VEC4',len(verts))
ix=data(struct.pack('<%sH'%(len(triangles)*3),*(q for t in triangles for q in t[:3])),5123,'SCALAR',len(triangles)*3)
ib=[]
for x,y,z in rest:ib.extend([1,0,0,0,0,1,0,0,0,0,1,0,-x,-y,-z,1])
ibm=data(struct.pack('<%sf'%len(ib),*ib),5126,'MAT4',23)
png=(A/'civilian_atlas.png').read_bytes()
while len(buf)%4:buf.append(0)
views.append({'buffer':0,'byteOffset':len(buf),'byteLength':len(png)});imageview=len(views)-1;buf.extend(png)
nodes=[]
for i in range(23):
 p=parent[i];r=rest[i];delta=[r[k]-(rest[p][k] if p>=0 else 0) for k in range(3)]
 node={'name':names[i],'translation':delta};children=[j for j in range(23) if parent[j]==i]
 if children:node['children']=children
 nodes.append(node)
nodes.append({'name':'Civilian mesh','mesh':0,'skin':0})
times=[i/12 for i in range(25)];ta=data(struct.pack('<25f',*times),5126,'SCALAR',25,([0],[2]))
sam=[];channels=[]
for j,phase in [(14,0),(18,math.pi)]:
 values=[q for t in times for q in (math.sin(math.sin(t*math.pi*2+phase)*.22),0,0,math.cos(math.sin(t*math.pi*2+phase)*.22))]
 oa=data(struct.pack('<100f',*values),5126,'VEC4',25);sam.append({'input':ta,'output':oa,'interpolation':'LINEAR'});channels.append({'sampler':len(sam)-1,'target':{'node':j,'path':'rotation'}})
# Rest A-pose in exported nodes uses shoulder Z rotations just like runtime.
for j,s in [(7,-1),(11,1)]:nodes[j]['rotation']=[0,0,math.sin(s*.65),math.cos(s*.65)]
gltf={'asset':{'version':'2.0','generator':'OKNO original mesh authoring v099','copyright':'CC0 1.0; original work generated for this project'},'scene':0,'scenes':[{'nodes':[22,23]}],'nodes':nodes,'skins':[{'name':'OKNO_Humanoid','joints':list(range(23)),'skeleton':22,'inverseBindMatrices':ibm}],'meshes':[{'name':'Civilian_LOD0','primitives':[{'attributes':{'POSITION':pa,'TEXCOORD_0':uv,'JOINTS_0':jo,'WEIGHTS_0':we},'indices':ix,'material':0}]}],'materials':[{'name':'Worn cloth atlas','doubleSided':True,'pbrMetallicRoughness':{'baseColorTexture':{'index':0},'metallicFactor':0,'roughnessFactor':1}}],'textures':[{'source':0,'sampler':0}],'samplers':[{'magFilter':9728,'minFilter':9984}],'images':[{'bufferView':imageview,'mimeType':'image/png'}],'animations':[{'name':'Walk_leg_reference','samplers':sam,'channels':channels}],'buffers':[{'byteLength':len(buf)}],'bufferViews':views,'accessors':acc}
j=json.dumps(gltf,separators=(',',':')).encode();j+=b' '*((-len(j))%4);buf+=b'\0'*((-len(buf))%4)
(A/'civilian_rig.glb').write_bytes(struct.pack('<III',0x46546c67,2,12+8+len(j)+8+len(buf))+struct.pack('<II',len(j),0x4e4f534a)+j+struct.pack('<II',len(buf),0x004e4942)+buf)
(A/'LICENSE_v098_geometry_history.txt').write_text('Original OKNO v099 civilian geometry, skeleton, atlas and generator output: CC0 1.0. No STALKER model, texture or animation was copied. No claim of STALKER bone-name/retargeting compatibility.\n')
(A/'mesh_manifest.json').write_text(json.dumps({'vertices':len(verts),'triangles':len(triangles),'joints':23,'names':names,'texture':'512x512; 128px face; four cloth variants','weights_normalized':all(0<=v[3]<=1 for v in verts),'stalker_retargeting_tested':False},indent=2))
print(len(verts),'vertices',len(triangles),'triangles',23,'joints')
