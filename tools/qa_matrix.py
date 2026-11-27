#!/usr/bin/env python3
"""Run 60 finite deterministic QA profiles (not AI agents).
No network, no recursion. Failures produce a reproducible repair prompt.
"""
import argparse, concurrent.futures, json, pathlib, subprocess, time
ROOT=pathlib.Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--rounds',type=int,default=1)
p.add_argument('--workers',type=int,default=4)
a=p.parse_args()
if not 1<=a.rounds<=10 or not 1<=a.workers<=8:p.error('rounds 1..10; workers 1..8')
out=ROOT/'release_v099'/'matrix';out.mkdir(parents=True,exist_ok=True)
subprocess.run(['gcc','-O2',str(ROOT/'tools/qa_v092.c'),'-lm','-o',str(ROOT/'build/qa_v092')],check=True)
all_results=[]
def run_profile(profile):
 work=out/f'round_{r:02d}'/f'profile_{profile:02d}';work.mkdir(parents=True,exist_ok=True)
 start=time.monotonic()
 result=subprocess.run([str(ROOT/'build/qa_v092'),str(profile)],cwd=work,text=True,capture_output=True,timeout=120)
 (work/'stdout.log').write_text(result.stdout);(work/'stderr.log').write_text(result.stderr)
 lines=[x for x in result.stdout.splitlines() if x.startswith('{')]
 row=json.loads(lines[-1]) if lines else {'profile':profile,'failures':1,'assertions':0}
 row.update(round=r,exit_code=result.returncode,elapsed_seconds=round(time.monotonic()-start,3))
 if result.returncode:
  (work/'REPAIR_PROMPT_RU.md').write_text(f'# Регрессия OKNO 0.9.2\n\nПрофиль {profile}, категория {profile%12}, seed {row.get("seed")}.\n\nВоспроизведение: build/qa_v092 {profile}\n\n```text\n{result.stderr}\n```\nИсправь причину, не ослабляя тест, и повтори полный набор.\n')
 return row
for r in range(a.rounds):
 with concurrent.futures.ThreadPoolExecutor(max_workers=a.workers) as executor:
  rows=list(executor.map(run_profile,range(60)))
 all_results.extend(rows)
 (out/'results.json').write_text(json.dumps(all_results,ensure_ascii=False,indent=2))
 failed=[row for row in rows if row['exit_code'] or row['failures']]
 print(f'Round {r+1}: {len(rows)-len(failed)}/60 profiles passed',flush=True)
 # Finite automatic reruns do not change source code or conceal failures.
 if failed:break
summary={'profiles':len(all_results),'assertions':sum(x['assertions'] for x in all_results),'failed_profiles':sum(bool(x['exit_code'] or x['failures']) for x in all_results),'ai_agents_created':0,'finite':True}
(out/'summary.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary),flush=True)
raise SystemExit(bool(summary['failed_profiles']))
