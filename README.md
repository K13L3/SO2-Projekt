# SO2-Projekt
## Problem jedzących filozofów
Program symuluje problem jedzących filozofów, który jest prezentacją problemu sychnronizacji pracujących współbieżnie procesów.

## Użyte mechanizmy
### 1. Semafor
Kontroluje liczbę filozofów, którzy mogą  jednocześnie siedzieć przy stole.
### 2. Mutexy do widelców
- Każdy widelec jest blokowany przez filozofa przed jedzeniem
- Filozofowie zawsze wybierają widelec o niższym numerze, aby zapobiec zakleszczeniu
### 3. Atomowe zmiany stanu
Przechowywują bieżacy stan filozofa i umożliwają bezpieczyn dostęp między wątkami
## Sekcje krytyczne
W programie sekcje krytyczne dotyczą dostępu do wspólnych zasobów – widelców oraz stołu. Dostęp do widelców jest synchronizowany za pomocą mutexów, a filozofowie zawsze podnoszą widelce w tej samej kolejności (najpierw ten o mniejszym numerze). Dodatkowo dodano semafor, który ogranicza liczbę filozofów mogących jednocześnie siedzieć przy stole do N-1, co eliminuje możliwość wzajemnego blokowania się.

## Uruchomienie programu
Do uruchomiena programu można użyć polecenia w terminalu:
make run, a następnie ./dining_philosophers.
