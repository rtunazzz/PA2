# Zadání
Tento program umožní vytvářet kvízy a testovat znalosti.
# Specifikace programu
Program bude implementovat následující:
  1. Alespoň 3 různé druhy otázek:
      - **Textová**: uživatel zadá odpověď psáním na klávesnici
      - **Single-choice**: uživatel vybere jednu z odpovědí
      - **Multi-choice**: uživatel vybere více z odpovědí
  2. Alespoň 3 různé druhy vyhodnocení odpovědi:
      - **Exact-match**: Odpověď je správná pouze pokud odpovídá konkrétnímu textu (např. uživatel musí zadat: `červená`)
      - **Pattern**: Odpověď je správná pokud bude odpovídat nějakému regexu (např. pokud mý být správná odpověď `červen*`, tak bude uznáno `červená`, `červené`, `červenou`, `červenat` atd.)
      - **Set**: Odpověď je správná pokud je obsažena v nějaké množině (např. bude uznán jakákoliv z `{ červená, modrá, bílá }`)
  3. Stránky kvízu, kvíz bude mít více stránek
  4. Sekvenční přechod mezi stránkami kvízu (tzn. následující otázka bude záležet na předchozí odpovědi)
  5. Uživatelské rozhraní pro tvorbu a vyplnění kvízu, trořené s pomocí knihovny SDL2.
  6. Import a export kvízu ze souboru

# Polymorfismus
Polymorfismus bute využit minimálně v:
### Reprezentaci různých typů otázek
Všechny typy otázky budou mít spolu mnoho věcí společných, ale také odlišných. Proto je zde výhodné využít polymorfismus.
Jako "kostra" otázek bude implemenotavá abstraktní třída `CQuestionBase`, ze které poté budou dědit ostatní typy otázek. Tato kostra bude definovat proměnné a metody které všechny typy otázek mají společné, jako např. `m_Type` (reprezentujíci typ otázky), `m_Question` (reprezentující otázku ve formě věty) a další.
Ne všechny implementace metod ale budou mít úplně stejné způsoby vyhodnocování, jako např. metoda `CQuestionBase::isCorrect` jejíž způsob vyhodnocení bude záležet na typu otázky.
### Reprezentaci různých typů odpovědí
Všechny typy odpovědí spolu také budou mít mnoho věcí společných.
Jako "kostra" všech odpovědí bude implementovaná abstraktní třída `CAnswerBase`. Tato třída bude implementovat všechny sdílené proměnné a metody a jako např.`CAnswerBase::isCorrect`
### Reprezentace výstupu
Uživatelské rozhraní bude implementováno s pomocí knihovny SDL2. Bude ale implementované takovým způsobem, že k případnému přidání dalšího rozhraní (např. terminálové, HTML apod.) nebude třeba měnit již implementované části kódu.
