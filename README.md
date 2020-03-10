Problema incepe prin a citi raza si numarul de spiridusi. Ulterior, se citeste
matricea ghetar si se initializeaza cu -1 (indicatie ca e libera). Apoi se
citesc spiridusii si caracteristicile lor si se memoreaza intr-un vector. In
final, se citesc comenzile pana la terminarea fisierului.

Comanda MOVE realizeaza miscarile spiridusilor sus, jos, stanga, dreapta daca
acestia au suficienta stamina. Daca spiridusul se muta intr-o casuta din afara
ghetarului, acesta iese din joc. Daca spiridusul se muta intr-o casuta deja
ocupata, el schimba manusile cu cele gasite acolo (daca sunt mai bune) si se
bate cu ocupantul pana unul dintre ei pierde hp-ul si iese din joc. Daca
spiridusul se muta intr-o celula libera, el schimba manusile cu cele gasite
daca sunt mai bune).

Comanda SNOWSTORM scade hp-ul spiridusilor daca acestia se afla in raza
furtunii. Epicentrul, raza si damage-ul se afla prin shiftari pe biti ale
numarului ulterior comenzii la citire. Daca un spiridus ramane fara hp, acesta
este eliminat din joc.

Comanda PRINT_SCOREBOARD afiseaza clasamentul spiridusilor in functie de
statusul lor (in joc sau nu), numarul de spiridusi pe care i-au eliminat si
in ordine alfabetica.

Comanda MELTDOWN reduce ghetarul cu o unitate din raza. Daca spiridusii nu se
mai afla pe ghetar, acestia ies din joc. Daca raman pe ghetar, energia lor creste cu un numar aflat la citire ulterior comenzii.

Jocul se termina daca mai ramane doar un spiridus pe ghetar (verificare in
functia win) sau daca nu se mai pot citi comenzi din fisier.
