Úkolem je vytvořit třídy, které budou modelovat matriku obyvatel. Realizované třídy umožní vyhledávání a mapování příbuzenských vztahů obyvatel.

V matrice ukládáme objekty reprezentující muže (CMan) a ženy (CWoman). Pro zjednodušení je datový model zatím takto neprogresivní. Je ale připraven na pokroky v oblasti výzkumu genderu a je otevřen pro snadné přidávání dalších pohlaví. Technicky je toho dosaženo polymorfismem - třídy CMan a CWoman budou podtřídy nějaké základní třídy (ze které by v budoucnu mohly být odvozeny i třídy pro další pohlaví). Přesný návrh tříd a jejich rozhraní je ponechán na Vás. Následující rozhraní je ale závazné:

CMan
Třída reprezentuje muže. Muž je reprezentován svým jménem (libovolný řetězec, může se opakovat) a jednoznačným identifikátorem (celé číslo int, neopakuje se). Dále může být vhodné ve třídě nějak ukládat informace o rodičích a dětech, případně další informace. Veřejné rozhraní třídy musí splňovat:

konstruktor s parametry (ID, name), který inicializuje instanci,
metodu GetID(), která vrátí ID,
operátor << pro výpis do streamu. Formát výpisu je formálně ID: name (man), viz ukázky běhu,
metodu PrintDescendants ( os ), která zobrazí potomky zadaného muže ve stromové podobě (viz ukázkové běhy). Ve výpisu jsou potomci vždy zobrazováni v pořadí, ve kterém byli přidaní do registru,
další metody, které se hodí pro Vaši implementaci.
CWoman
Třída reprezentuje ženu. Žena je reprezentován svým jménem (libovolný řetězec, může se opakovat) jednoznačným identifikátorem (celé číslo int, neopakuje se) a případnými svými dřívějšími jmény (rodné jméno, jména z předešlých manželství). Dále může být vhodné ve třídě nějak ukládat informace o rodičích a dětech a případně i další informace. Veřejné rozhraní třídy musí splňovat:

konstruktor s parametry (ID, name), který inicializuje instanci,
metodu GetID(), která vrátí ID,
operátor << pro výpis do streamu. Formát výpisu je formálně ID: name (woman) nebo ID: name [maidenName, name#1, name#2, ..., name#N] (woman) pokud se žena vdala, viz ukázky běhu. Pokud se zobrazuje seznam dřívějších jmen, pak tento seznam vždy začíná rodným jménem a pokračuje jmény z dřívějších manželství v pořadí, ve kterém se jména měnila/přidávala,
metodu PrintDescendants, která zobrazí potomky zadané ženy ve stromové podobě (viz ukázkové běhy). Ve výpisu jsou potomci vždy zobrazováni v pořadí, ve kterém byli přidaní do registru,
metoda Wedding (newName) změní jméno ženy po svatbě. Chování této metody je následující:
aktuální jméno se uloží na konec seznamu dříve používaných jmen,
jméno ženy se změní na jméno z parametru,
pokud na instanci CWoman dosud nebyla zavolaná metoda Wedding, bude se instance zobrazovat bez informace o dřívějších jménech (formát bez hranatých závorek a seznamu). Po zavolání metody Wedding se bude instance zobrazovat s informací o předchozích jménech (formát s hranatými závorkami a seznamem). V extrémním případě se může stát, že volání Wedding bude mít parametr newName stejný jako aktuální jméno ženy. I toto ale považujeme za změnu jména a dřívější jméno uložíme. V extrémním případě by tak mohl výpis mít i podobu např. 666: Novakova [Novakova, Novakova] (woman) (rozená "Novakova", 2x svatba se změnou jména na "Novakova").
další metody, které se hodí pro Vaši implementaci.
CRegister
Třída reprezentuje registr obyvatelstva (matriku). Obsahuje metody, které umožní přidávat informace do matriky a v matrice vyhledávat. Její veřejné rozhraní musí obsahovat následující metody:

implicitní konstruktor, který vytvoří prázdnou matriku,
metodu Add (person, father, mother), která přidá do matriky další záznam. Parametrem je přidávaná osoba (person), osoba je přidávaná jako instance CMan nebo CWoman zabalená ve smart pointeru shared_ptr. Další dva parametry jsou odkazy na rodiče, pokud jsou tyto informace k dispozici. Pokud rodič není znám/v matrice neexistuje, bude mít odpovídající parametr hodnotu nullptr. Metodu lze volat i v kombinaci, kdy je známý/dostupný pouze jeden z rodičů (ten je pak vyplněn) a druhý známý není (bude nullptr). Metoda vrátí hodnotu true, pokud uspěje nebo false, pokud dojde k chybě. Za chybu považujte, pokud se pokusíme přidat osobu s již existujícím ID. Není potřeba kontrolovat, že zadaný otec je instancí CMan a matka instancí CWoman. Testovací prostředí není dostatečně progresivní, aby takové vstupy zadávalo.
metodu FindByID (ID), která vyhledá osobu podle zadaného ID. Metoda vrátí odkaz na odpovídající osobu, případně vrátí neplatný odkaz, pokud osoba se zadaným ID neexistuje. Návratovou hodnotou musí být typ, který lze dereferencovat a zpřístupnit jím instanci CMan/CWoman, tedy ukazatel, shared_ptr či vlastní implementace smart pointeru.
metodu FindByName(prefix), která vyhledá v matrice osoby, jejichž jména začínají zadaným prefixem. Návratovou hodnotou je vektor odkazů na osoby, které tomuto kritériu vyhovují. Osoby ve výsledném vektoru budou uspořádané vzestupně podle jejich ID. Pokud zadanému kritériu nevyhoví žádná osoba v matrice, bude výsledkem prázdný vektor. Při hledání se kontroluje jak jméno, tak i případná dřívější jména u instancí CWoman. Konečně, výsledný vektor nesmí obsahovat duplicity. Například, pokud se hledá s prefixem "Novakova" a instance CWoman má jméno i rodné jméno "Novakova", bude tato instance ve výsledku zahrnuta pouze jednou. Při porovnání rozlišujte malá a velká písmena (case-sensitive).
metoda FindRelatives ( id1, id2 ), která vyhledá příbuzenský vztah mezi osobami se zadanými identifikátory. Výsledkem metody může být:
prázdný seznam, pokud mezi zadanými osobami neexistuje žádný příbuzenský vztah,
seznam osob, které popisují nalezený příbuzenský vztah. Prvkem seznamu je vždy dvojice - odkaz na osobu a příbuzenský vztah. Prvním prvkem výsledného seznamu je první bezprostřední příbuzný osoby id1 s informací o příbuzenském vztahu této osoby k id1 (pozor, samotná osoba s id1 ve výsledném seznamu není). Následují následují odkazy na mezilehlé přímé příbuzné a seznam končí odkazem na osobu s id2. V každém prvku je pak informace o příbuzenském vztahu k osobě předcházející. Dvojice sousedních osob ve vráceném seznamu musí mít přímý příbuzenský vztah, tedy musí se jednat o vztah syn/dcera/otec/matka (složitější vztahy musí být v seznamu rozepsány, tedy už například bratr by musel být zapsán pomocí mezilehlého otce/matky). Příbuzenských vztahů pro zadanou dvojici osob na vstupu může existovat více, metoda musí vyhledat ten z nich, který je nejkratší. Ani nejkratší příbuzenský vztah není určen jednoznačně, metoda proto může vrátit libovolný takový nejkratší vztah (testovací prostředí zkontroluje délku faktickou pravdivost vrácené posloupnosti příbuzných),
metoda vyhodí výjimku std::invalid_argument, pokud zadané id1 nebo id2 není v matrice známé nebo pokud id1 == id2.
ERel
Výčtový typ definuje konstanty, které určují přímý příbuzenský vztah (syn/dcera/otec/matka). Tento výčet je deklarovaný v testovacím prostředí, Vaše implementace jej musí použít v nezměněné podobě (při odevzdání jej ponechte v bloku podmíněného překladu).

Odevzdávejte soubor, který obsahuje implementaci požadovaných tříd a další Vaše podpůrné třídy. Třídy musí splňovat veřejné rozhraní podle ukázky - pokud Vámi odevzdané řešení nebude obsahovat popsané rozhraní, dojde k chybě při kompilaci. Do tříd si ale můžete doplnit další metody (veřejné nebo i privátní) a členské proměnné. Odevzdávaný soubor musí obsahovat jak deklaraci třídy (popis rozhraní) tak i definice metod, konstruktoru a destruktoru. Je jedno, zda jsou metody implementované inline nebo odděleně. Odevzdávaný soubor nesmí obsahovat vkládání hlavičkových souborů a funkci main. Funkce main a vkládání hlavičkových souborů může zůstat, ale pouze obalené direktivami podmíněného překladu jako v ukázce níže.

Při řešení úlohy využijte STL, seznam povolených hlavičkových souborů je obsažen v přiloženém zdrojovém souboru se základem implementace.

Úloha je hodnocena v několika testech, podle počtu zvládnutých testů pak Vaše řešení získá vyšší či nižší hodnocení. Pokud se rozhodnete odevzdávat neúplné řešení (např. bude chybět implementace CRegister::FindRelatives), doplňte prázdná těla neimplementovaných metod, případně vracejte nějaké "dummy" výsledky (např. prázdný seznam pro CRegister::FindRelatives). Pokud byste vynechali implementaci úplně, odevzdaný kód by se nedal zkompilovat. Jednotlivé testy kontrolují:

Návrh polymorfních tříd, metody CMan::GetID, výpis CMan, CWoman::GetID, CWoman::Wedding, výpis CWoman, CRegister::Add, CRegister::FindByID, CRegister::FindByName. Tento test je povinný, jeho nezvládnutí znamená hodnocení 0 bodů.
Navíc CMan::PrintDescendants a CWoman::PrintDescendants.
Navíc CRegister::FindRelatives.
Časová efektivita CRegister::FindByName.
Nápověda:
Určitě začněte návrhem tříd a polymorfismu. Bez polymorfismu program nepůjde zkompilovat/neprojde povinným testem.
Využijte vhodné algoritmy a datové struktury ze standardní knihovny, viz seznam dostupných hlavičkových souborů.
Přiložený ukázkový soubor ukazuje jeden z možných výsledků volání CRegister::FindRelatives. Pokud Vaše implementace nalezne jiný správný příbuzenský vztah, bude v přiloženém testovacím souboru výsledek označen za neplatný. Ale testovací prostředí na Progtestu takový správný výsledek uzná.
Parametry father a mother u metody CRegister::Add, návratový typ metody CRegister::FindByID a další metody pracují s odkazy na instance CMan / CWoman. Můžete je implementovat pomocí ukazatelů, smart pointerů ze std. knihovny, případně i nějaké své obalující třídy s vlastností "ukazatele". Důležité je, aby návratový typ šlo dereferencovat, použít v podmínce (platný/neplatný) a inicializovat z nullptr. Dále je potřeba, aby Vaše rozhraní bylo konzistentní (tj. všude používejte buď ukazatele, nebo všude používejte smart pointery). Výjimka je parametr person u CRegister::Add, kde vždy musíte používat smart pointer ze std. knihovny.