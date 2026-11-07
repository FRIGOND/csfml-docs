#!/usr/bin/env python3
"""Génère un programme qui appelle chaque sfXxx_destroy(NULL) dans un processus fils.
Usage: python3 -I gen_destroy.py <dossier include> <prefix include: CSFML|SFML>"""
import re,sys,glob,os
inc,pfx=sys.argv[1],sys.argv[2]
names=[]
for f in sorted(glob.glob(f"{inc}/{pfx}/*/*.h")):
    t=open(f,encoding='utf-8').read()
    for m in re.finditer(r'CSFML_\w+_API\s+void\s+(sf\w+_destroy)\s*\(',t): names.append(m.group(1))
names=sorted(set(names))
print(f"#include <{pfx}/Audio.h>\n#include <{pfx}/Graphics.h>\n#include <{pfx}/Network.h>\n#include <{pfx}/System.h>\n#include <{pfx}/Window.h>")
print("#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <sys/wait.h>\n#include <unistd.h>")
print("typedef void (*fn_t)(void *);\nstruct e {const char *n; fn_t f;};\nstatic struct e tab[] = {")
for n in names: print(f'  {{"{n}", (fn_t){n}}},')
print("};\nint main(void){ int ok=0,bad=0; setvbuf(stdout,NULL,_IONBF,0);\n for(size_t i=0;i<sizeof tab/sizeof *tab;i++){ pid_t p=fork(); if(!p){ freopen(\"/dev/null\",\"w\",stderr); tab[i].f(NULL); _exit(0);} int st; waitpid(p,&st,0);\n  if(WIFEXITED(st)&&WEXITSTATUS(st)==0) ok++; else {bad++; printf(\"  CRASH %s (signal %d)\\n\",tab[i].n,WIFSIGNALED(st)?WTERMSIG(st):-1);} }\n printf(\"total=%zu ok=%d crash=%d\\n\",sizeof tab/sizeof *tab,ok,bad); return 0;}")
