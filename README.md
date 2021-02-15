# Repozitář PA2

Ahoj studente,

toto je tvůj oficiální repozitář k předmětu PA2. Repozitář ti byl vygenerován automaticky, dle doporučené struktury, ale ty si jej můžeš upravit dle svého uvážení. Případně to zkonzultuj se svými cvičícími.

Repozitář již obsahuje 3 adresáře:

```
homeworks/ ..... domácí úlohy
practice/ ...... procvičovací úlohy
semestral/ ..... semestrální úloha
```

Využívat jej je nepovinné, ale doporučované.

## Co tu je za soubory?

Jistě sis všiml, že soubor `README.md` má v systému Gitlab své speciální místo. Zobrazuje se při procházení adresáři a měl by obsahovat nějaké stručné informace.

Dále sis asi všiml souborů `.gitkeep`. Nástroj `git` bohužel umí verzovat jen soubory, nikoli adresáře, takže aby si mohl verzovat prázdný adresář, musíš do něj něco vložit. A běžně se můžeš setkat s podobnými soubory jako `.gitkeep`, které slouží přesně k tomuto účelu. Samozřejmě je můžeš smazat, jen si pak budeš muset složku případně vytvořit ručně.

Můžeš tu najít také soubor `.gitignore`, ten je velice důležitý. Definuje, jaké soubory se nemají verzovat. My jsme ho pro tebe už vyplnili doporučeným nastavením pro jazyk C/C++. Takže například soubory, které končí na `.so` nebo `.o` nebo `.out` se verzovat nebudou. Ale samozřejmě si ho můžeš upravit podle sebe.

> Při verzování se vždy zamysli, zda soubor verzovat musíš. Binární soubory, které vygeneruješ ze zdrojového kódu kompilací je zbytečné verzovat, vždy totiž můžeš zdrojový kód zkompilovat.

Poslední soubor, který tu najdeš je `.editorconfig`. Ten se netýká Gitu, ale mnoho editorů podporuje tento formát pro nastavení základní coding-style. My už jsme tam hodili nějaká doporučená nastavení, ale klidně si jej uprav podle sebe.
