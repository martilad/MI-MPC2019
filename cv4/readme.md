# task4 - Chyba Microsoft implementace C++?

## Zadání - z hodiny (byly přiloženy nápovědy a odkazy do standardu C++ pro snadnější orientaci)
 
Uvažujte následující kód:


```
#include <map>
#include <vector>

struct X // non-copyable
{
    X(X&&) = default;
    X(const X&) = delete;
};

struct Y
{
    std::map<int, X> map;
 // Y(Y&&) = default;
};

void f()
{
    std::vector<Y> v;
    v.reserve(1);
}
```

Tento kód překladače GCC i Clang přeloží bez problému. S překladačem MSVC ale skončí překlad s chybou. Zkoušeno s verzí MSVC 19.22 pro architekturu x64. Viz např.: https://godbolt.org/z/9KBJP2.
Navíc, při odkomentování řádku s uživatelskou definicí přesouvacího konstruktoru Y(Y&&) = default; skončí překlad bez chyby i s překladačem MSVC.

### Otázky pro vyřešení:
 * Jedná se o chybu (bug) v Microsoft implementaci C++, nebo se tato implementace chová korektně podle Standardu C++? V obou případech odpověď zdůvodněte.
 * Vysvětlete, proč odkomentování definice přesouvacího konstruktoru Y::Y(Y&&) způsobí překlad bez chyby.
 
## Řešení

První se pokusím začít otázkou 2.
Pokud se vytváří move konstruktor implicitní (v Y) je vytvořen také
kopírující konstruktor (implicitní).
Podle standardu není nutné, aby move konstruktor byl noexcept.
Odpovědi na otázky z nápovědy. Přesouvací konstruktor Y není noexcept a
přesouvací konstruktor std::map v MSVC není s noexcept specifikací.
Při realokaci a využití std::move_if_noexcept je proto preferován
kopírující konstruktor, který bude volán před přesouvacím konstruktorem.

Kopírující konstruktror v X je deleted a překlad skončí s chybou.

Pokud odkomentuji řádku "Y(Y&&) = default;", tak kód začne fungovat
proto, že ve větě [class.copy.ctor/6/2]:
http://eel.is/c++draft/class.copy.ctor#6.sentence-2. je řečeno, že pokud
je deklarován move konstruktor uživatele, tak copy construktor je
označen jako deleted, nebude teda volán a bude volám move construktor, i
když není s noexcept a překlad proběhne v pořádku.
To je odpoevěď proč proběhne překlad poté v pořádku, ale proč v GCC a
Clang proběhne i bez té řádky?
Proč i první verze funguje v GCC a CLANG? To je možné nejspíš díky větě
v [res.on.exception.handling/5]:
http://eel.is/c++draft/conforming#res.on.exception.handling-5. Standard
dovoluje zesílit deklaraci implicitního move konstruktoru na noexcept, a
i GCC i CLANG to zřejmě využívají a implicitní move konstruktor je s
noexcept, proto potom díky std::move_if_noexcept použíjí move
konstruktor a překlad nevyvolá chyby ani když nebude uveden řádek
"Y(Y&&) = default;", který by dle standartu zařídil to, že by byl
kopírující konstruktor delete.

Zpětně tedy odpověď na otázku 1 je, že se nejedná o bug v MSVC, který
funguje podle standardu, pouze v GCC a CLANG bylo implementováno
zesílení, díky kterému nedojde v první variantě k chybě v překladu.
