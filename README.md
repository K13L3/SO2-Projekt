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


## Uruchomienie programu
Do uruchomiena programu można użyć polecenia w terminalu:
make run, a następnie ./dining_philosophers.
