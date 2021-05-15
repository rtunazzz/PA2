Úkolem je vytvořit třídy, které budou modelovat matriku obyvatel. Realizované třídy umožní vyhledávání a zjišťování informací o odchodu do penze.

V matrice ukládáme objekty reprezentující muže (CMan) a ženy (CWoman). Pro zjednodušení je datový model zatím takto neprogresivní. Je ale připraven na pokroky v oblasti výzkumu genderu a je otevřen pro snadné přidávání dalších pohlaví. Technicky je toho dosaženo polymorfismem - třídy CMan a CWoman budou podtřídy nějaké základní třídy (ze které by v budoucnu mohly být odvozeny i třídy pro další pohlaví). Přesný návrh tříd a jejich rozhraní je ponechán na Vás. Následující rozhraní je ale závazné:

CMan
Třída reprezentuje muže. Muž je reprezentován svým jménem (libovolný řetězec, může se opakovat), jednoznačným identifikátorem (celé číslo int, neopakuje se) a datem narození. Dále může být vhodné ve třídě nějak ukládat informace o rodičích a dětech, případně další informace. Veřejné rozhraní třídy musí splňovat:

konstruktor s parametry (ID, name, dateBorn), který inicializuje instanci,
metodu GetID(), která vrátí ID,
operátor << pro výpis do streamu. Formát výpisu je formálně ID: name, man, born: date, retires: date, viz ukázky běhu,
metodu ScanPedigree (), která vyhledá jména všech osob - známých předků, které v registru obyvatel evidujeme. Jejich jména vrátí jako množinu řetězců,
metodu Military ( days ), která pro daného muže eviduje dobu strávenou ve vojenském výcviku či na vojenském cvičení. Metoda může být pro danou instanci volaná vícekrát (účast na více vojenských cvičeních). Evidence vojenských cvičení se projeví ve výpočtu doby odchodu do důchodu (viz dále),
další metody, které se hodí pro Vaši implementaci.
CWoman
Třída reprezentuje ženu. Žena je reprezentován svým jménem (libovolný řetězec, může se opakovat) jednoznačným identifikátorem (celé číslo int, neopakuje se) a datem narození. Dále může být vhodné ve třídě nějak ukládat informace o rodičích a dětech a případně i další informace. Veřejné rozhraní třídy musí splňovat:

konstruktor s parametry (ID, name, dateBorn), který inicializuje instanci,
metodu GetID(), která vrátí ID,
operátor << pro výpis do streamu. Formát výpisu je formálně ID: name, woman, born: date, retires: date, viz ukázky běhu,
metodu ScanPedigree (), která vyhledá jména všech osob - známých předků, které v registru obyvatel evidujeme. Jejich jména vrátí jako množinu řetězců,
další metody, které se hodí pro Vaši implementaci.
CRegister
Třída reprezentuje registr obyvatelstva (matriku). Obsahuje metody, které umožní přidávat informace do matriky a v matrice vyhledávat. Její veřejné rozhraní musí obsahovat následující metody:

implicitní konstruktor, který vytvoří prázdnou matriku,
kopírující konstruktor, který vytvoří identickou kopii zdrojové matriky. Kopírující konstruktor implementujte pouze pokud kompilátorem dodaná implementace nevyhoví,
destruktor, který uvolní alokované prostředky. Destruktor implementujte pouze pokud kompilátorem dodaná implementace nevyhoví,
kopírující operátor = není při testování používán. Můžete jej pro jistotu zakázat (=delete),
metodu Add (person, father, mother), která přidá do matriky další záznam. Parametrem je přidávaná osoba (person), osoba je přidávaná jako instance CMan nebo CWoman zabalená ve smart pointeru shared_ptr. Další dva parametry jsou odkazy na rodiče, pokud jsou tyto informace k dispozici. Pokud rodič není znám/v matrice neexistuje, bude mít odpovídající parametr hodnotu nullptr. Metodu lze volat i v kombinaci, kdy je známý/dostupný pouze jeden z rodičů (ten je pak vyplněn) a druhý známý není (bude nullptr). Metoda vrátí hodnotu true, pokud uspěje nebo false, pokud dojde k chybě. Za chybu považujte, pokud se pokusíme přidat osobu s již existujícím ID. Není potřeba kontrolovat, že zadaný otec je instancí CMan a matka instancí CWoman. Testovací prostředí není dostatečně progresivní, aby takové vstupy zadávalo. Dále, není potřeba kontrolovat data narození nebo věk rodičů, matrika je v tomto velmi benevolentní. Dokonce připustíme, aby se nově narozená osoba narodila před datem narození svých rodičů,
metodu FindByID (ID), která vyhledá osobu podle zadaného ID. Metoda vrátí odkaz na odpovídající osobu, případně vrátí neplatný odkaz, pokud osoba se zadaným ID neexistuje. Návratovou hodnotou musí být typ, který lze dereferencovat a zpřístupnit jím instanci CMan/CWoman, tedy ukazatel, shared_ptr či vlastní implementace smart pointeru.
metodu FindRetired( from, to ), která vyhledá v matrice osoby, které budou odcházet do důchodu v zadaném časovém intervalu. Do výsledku vložte i osoby, které budou odcházet do důchodu v hraničních dnech zadaného intervalu. Pokud zadanému kritériu nevyhoví žádná osoba v matrice, bude výsledkem prázdný vektor. Výsledný vektor bude obsahovat odkazy na osoby vyhovující zadanému kritériu, osoby budou seřazené vzestupně podle svých identifikátorů.
CDate
Jednoduchá třída reprezentuje datum. Testování používá pouze konstruktor, který vyplní instanci zadanými hodnotami roku, měsíce a dne. Do třídy si doplňte další rozhraní, které bude potřeba pro vaši implementaci. Můžete se spolehnout, že testovací prostředí zadává pouze platná data. Pro jednoduchost pracujeme s kalendářem, ve kterém neexistují přestupné roky, tedy únor má vždy 28 dní.

Pravidla pro výpočet odchodu do důchodu v tomto příkladu jsou vymyšlená, nemají moc společného s realitou. Při výpočtu se uplatní různá kritéria pro muže a ženy:

Muži jdou do důchodu 65 let od narození. Od této doby se ale odečítá doba strávená na vojně a při následných vojenských cvičeních takto:
doba strávená na nejdelším vojenském cvičení se odečte 4x,
doba strávená na druhém nejdelším vojenském cvičení se odečte 3x,
doba strávená na třetím nejdelším vojenském cvičení se odečte 2x,
ostatní vojenská cvičení se odečítají podle své délky bez zvýhodnění,
dále se odečte 10 dní, za každého syna, který byl alespoň 1 den na vojenském cvičení,
maximálně lze odečíst 20 let.
Například pro zadané délky cvičení 80, 230, 180, 15, 180 a 60 dní a dva syny na vojenském cvičení se celkem odečte: (4*230 + 3*180 + 2*180 + 80 + 15 + 60 + 2*10). Protože výsledný součet je nižší než 20 let, sníží se o tento počet dní doba věku odchodu do důchodu.
Ženy odcházejí do důchodu ve věku 60 let, ale za každé dítě se oba odchodu do důchodu zkracuje o 4 roky. Dále se doba odchodu do důchodu zkracuje o 10 dní za každého syna, který byl alespoň 1 den na vojenském cvičení. Maximálně lze odečíst 20 let.
Odevzdávejte soubor, který obsahuje implementaci požadovaných tříd a další Vaše podpůrné třídy. Třídy musí splňovat veřejné rozhraní podle ukázky - pokud Vámi odevzdané řešení nebude obsahovat popsané rozhraní, dojde k chybě při kompilaci. Do tříd si ale můžete doplnit další metody (veřejné nebo i privátní) a členské proměnné. Odevzdávaný soubor musí obsahovat jak deklaraci třídy (popis rozhraní) tak i definice metod, konstruktoru a destruktoru. Je jedno, zda jsou metody implementované inline nebo odděleně. Odevzdávaný soubor nesmí obsahovat vkládání hlavičkových souborů a funkci main. Funkce main a vkládání hlavičkových souborů může zůstat, ale pouze obalené direktivami podmíněného překladu jako v ukázce níže.

Při řešení úlohy využijte STL, seznam povolených hlavičkových souborů je obsažen v přiloženém zdrojovém souboru se základem implementace.

Úloha je hodnocena v několika testech, podle počtu zvládnutých testů pak Vaše řešení získá vyšší či nižší hodnocení. Pokud se rozhodnete odevzdávat neúplné řešení (např. bude chybět implementace CMan::ScanPedigree), doplňte prázdná těla neimplementovaných metod, případně vracejte nějaké "dummy" výsledky (např. prázdnou množinu pro CMan::ScanPedigree). Pokud byste vynechali implementaci úplně, odevzdaný kód by se nedal zkompilovat. Jednotlivé testy kontrolují:

Návrh polymorfních tříd, metody CMan::GetID, CMan::Military, výpis CMan, CWoman::GetID, výpis CWoman, CRegister::Add, CRegister::FindByID, CRegister::FindRetired. Tento test je povinný, jeho nezvládnutí znamená hodnocení 0 bodů.
Navíc CMan::ScanPedigree a CWoman::ScanPedigree.
Navíc se testuje správné kopírování instancí CRegister.
Časová efektivita CRegister::FindRetired.
Nápověda:
Určitě začněte návrhem tříd a polymorfismu. Bez polymorfismu program nepůjde zkompilovat/neprojde povinným testem.
Využijte vhodné algoritmy a datové struktury ze standardní knihovny, viz seznam dostupných hlavičkových souborů.
Nezávisle si naprogramujte a otestujte třídu pro práci s datem. Využijte a upravte kód, který máte hotový z cvičných úloh PA1/PA2.
Parametry father a mother u metody CRegister::Add, návratový typ metody CRegister::FindByID a další metody pracují s odkazy na instance CMan / CWoman. Můžete je implementovat pomocí ukazatelů, smart pointerů ze std. knihovny, případně i nějaké své obalující třídy s vlastností "ukazatele". Důležité je, aby návratový typ šlo dereferencovat, použít v podmínce (platný/neplatný) a inicializovat z nullptr. Dále je potřeba, aby Vaše rozhraní bylo konzistentní (tj. všude používejte buď ukazatele, nebo všude používejte smart pointery). Výjimka je parametr person u CRegister::Add, kde vždy musíte používat smart pointer ze std. knihovny.
