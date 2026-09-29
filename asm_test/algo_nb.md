# Algorithme itoa

Reçoit un entier dans `rax`, l'affiche sur stdout.

---

## Étapes

```
1. r8 = 0

2. si rax < 0 :
       rax = -rax
       int_buf[0] = '-'
       r8 = 1

3. rcx = 0
   tant que rax != 0 :
       rdx = rax % 10
       rax = rax / 10
       push(rdx + 48)
       rcx++

4. rsi = int_buf + r8
   répéter rcx fois :
       pop rdx
       *rsi = dl
       rsi++

5. write(1, int_buf, rcx + r8)
```

---

## Exemple : -34

**Étape 1 — init**
```
r8 = 0
```

**Étape 2 — signe**
```
rax = -34  ->  rax < 0
rax = 34
int_buf[0] = '-'
r8 = 1
```

**Étape 3 — décomposition**
```
tour 1 : rdx = 34 % 10 = 4   rax = 3   push('4'=52)   rcx = 1
tour 2 : rdx =  3 % 10 = 3   rax = 0   push('3'=51)   rcx = 2
rax == 0, on sort
pile (sommet -> fond) : 51 52
                        '3' '4'
```

**Étape 4 — dépile**
```
rsi = int_buf + 1   (on saute le '-')

pop -> '3'  →  int_buf[1] = '3'   rsi++
pop -> '4'  →  int_buf[2] = '4'   rsi++
```

**Buffer final**
```
int_buf: [ '-' | '3' | '4' ]
```

**Étape 5 — write**
```
write(1, int_buf, 2 + 1)  →  affiche "-34"
```

---

## Exemple : 42

**Étape 1 — init**
```
r8 = 0
```

**Étape 2 — signe**
```
rax = 42  ->  rax >= 0, on ne fait rien
r8 reste 0
```

**Étape 3 — décomposition**
```
tour 1 : rdx = 42 % 10 = 2   rax = 4   push('2'=50)   rcx = 1
tour 2 : rdx =  4 % 10 = 4   rax = 0   push('4'=52)   rcx = 2
rax == 0, on sort
pile (sommet → fond) : 52 50
                        '4' '2'
```

**Étape 4 — dépile**
```
rsi = int_buf + 0   (pas de signe, on commence au début)

pop -> '4'  ->  int_buf[0] = '4'   rsi++
pop -> '2'  ->  int_buf[1] = '2'   rsi++
```

**Buffer final**
```
int_buf: [ '4' | '2' ]
            0     1
```

**Étape 5 — write**
```
write(1, int_buf, 2 + 0)  →  affiche "42"
```
