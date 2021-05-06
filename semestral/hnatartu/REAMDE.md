# Zadání
Tento program umožní vytvářet kvízy a testovat znalosti.
## Specifikace programu
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
  5. Uživatelské rozhraní pro tvorbu a vyplnění kvízu, pravděpodobně trořené s pomocí knihovny SDL2.
  6. Import a export kvízu

## Polymorfismus
Polymorfismuss bute využit minimálně v:
  1. **Reprezentaci různých typů otázek** (abstraktní třída `CQuestionBase`, ze které poté budou dědit všechny typy otázek, jako např. `CSingleChoice`)
  2. **Reprezentaci různých typů odpovědí** (abstraktní třída `CAnswerBase`, ze které poté budou dědit všechny typy odpovědí, jako např. `CExactMatch`)
