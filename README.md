#SO_project_clinic 

##github repo: https://github.com/badgersky/SO_project_clinic

###Temat 15 - przychodnia
W przychodni przyjmują pacjentów dwaj lekarze POZ (pierwszego kontaktu) oraz lekarze o
specjalizacjach: kardiolog, okulista, pediatra i lekarz medycyny pracy. Każdy z lekarzy w danym dniu
może przyjąć określoną liczbę pacjentów: lekarze POZ każdy X1 osób, kardiolog X2 osób, okulista
X3 osób, pediatra X4 osób, lekarz medycyny pracy X5 osób. Lekarze POZ przyjmują ok. 60%
wszystkich pacjentów (wartość losowa), lekarze specjaliści każdy po ok. 10% wszystkich pacjentów
(wartość losowa). Do obu lekarzy POZ jest jedna wspólna kolejka pacjentów, do każdego lekarza
specjalisty kolejki są indywidualne.
Lekarze POZ część pacjentów (ok. 20% z X1) kierują na dodatkowe badania do wskazanych lekarzy
specjalistów – pacjent zostaje zarejestrowany do specjalisty przez lekarza POZ, jeżeli dany lekarz nie
ma już wolnych terminów dane pacjenta (id - skierowanie do …. - wystawił) powinny zostać zapisane
w raporcie dziennym. Lekarze specjaliści dodatkowo mogą skierować pacjenta (ok. 10%) na badanie
ambulatoryjne po wykonaniu którego pacjent wraca do danego specjalisty i wchodzi do gabinetu bez
kolejki (po uprzednim wyjściu pacjenta). Jeśli w chwili zamknięcia przychodni w kolejce do lekarza
czekali pacjenci to te osoby zostaną przyjęte w tym dniu ale nie mogą zostać skierowane na
dodatkowe badania.
Zasady działania przychodni ustalone przez Dyrektora są następujące:
- Przychodnia jest czynna w godzinach od Tp do Tk;
- W budynku przychodni w danej chwili może się znajdować co najwyżej N pacjentów
(pozostali, jeżeli są czekają przed wejściem);
- Dzieci w wieku poniżej 18 lat do przychodni przychodzą pod opieką osoby dorosłej;
- Każdy pacjent przed wizytą u lekarza musi się udać do rejestracji;
- W przychodni są 2 okienka rejestracji, zawsze działa min. 1 stanowisko;
- Jeżeli w kolejce do rejestracji stoi więcej niż K pacjentów (K>=N/2) otwiera się drugie okienko
rejestracji. Drugie okienko zamyka się jeżeli liczba pacjentów w kolejce do rejestracji jest
mniejsza niż N/3;
14
- Osoby uprawnione VIP (np. honorowy dawca krwi) do gabinetu lekarskiego wchodzą bez
kolejki;
- Jeżeli zostaną wyczerpane limity przyjęć do danego lekarza, pacjenci ci nie są przyjmowani
(rejestrowani);
- Jeżeli zostaną wyczerpane limity przyjęć w danym dniu do wszystkich lekarzy, pacjenci nie
są wpuszczani do budynku;
- Jeśli w chwili zamknięcia przychodni w kolejce do rejestracji czekali pacjenci to te osoby nie
zostaną przyjęte w tym dniu przez lekarza. Dane tych pacjentów (id - skierowanie do …
wystawił) powinny zostać zapisane w raporcie dziennym.

Na polecenie Dyrektora (sygnał 1) dany lekarz bada bieżącego pacjenta i kończy pracę przed
zamknięciem przychodni. Dane pacjentów (id - skierowanie do …. - wystawił), którzy nie zostali
przyjęci powinny zostać zapisane w raporcie dziennym.
Na polecenie Dyrektora (sygnał 2) wszyscy pacjenci natychmiast opuszczają budynek.
Napisz procedury Dyrektor, Rejestracja, Lekarz i Pacjent symulujące działanie przychodni

###Topic 15 - clinic
In the clinic, patients are attended by two general practitioners (GPs) and doctors with specialties:
cardiologist, ophthalmologist, pediatrician, and occupational medicine doctor. Each doctor in a given
day can attend to a specific number of patients: each GP can see X1 people, cardiologist X2 people,
ophthalmologist X3 people, pediatrician X4 people, occupational medicine doctor X5 people. GPs attend to
about 60% of all patients (a random value), each specialist attends to about 10% of all patients (a random value).
Both GPs share one common queue of patients, and each specialist has their own individual queue.
GPs refer some patients (about 20% of X1) for additional consultations to designated specialists – the patient
is registered to the specialist by the GP. If that specialist has no more available slots, the patient’s
data (id - referral to …. - issued by) should be recorded in the daily report. Specialists can also refer
a patient (about 10%) for an outpatient test, after which the patient returns to the given specialist and 
enters the office without waiting in line (after the current patient leaves). If, at the clinic’s closing
time, there are still patients waiting in a doctor’s queue, those patients will be seen on that day but
cannot be referred for additional consultations.
The rules for the clinic, as established by the Director, are as follows:
- The clinic is open from Tp to Tk;
- At any given moment, no more than N patients can be inside the clinic (others, if any, must wait outside);
- Children under the age of 18 must be accompanied by an adult to the clinic;
- Every patient must go to registration before visiting a doctor;
- There are 2 registration windows, with at least 1 always open;
- If there are more than K patients (K >= N/2) in the queue for registration, the second window opens.
- The second window closes if the number of patients in the registration queue drops below N/3;
- VIP patients (e.g., honorary blood donors) can enter the doctor’s office without waiting in line;
- If a doctor’s appointment slots are fully booked, those patients will not be admitted (registered);
- If all doctors’ appointment slots are fully booked for the day, patients are not allowed to enter the building;
- If, at the clinic’s closing time, there are still patients in the registration queue, they will not be seen that
- day by any doctor. The data of these patients (id - referral to … - issued by) should be recorded in the daily report.

On the Director’s order (signal 1), a given doctor examines the current patient and finishes work before the clinic
closes. The data of patients (id - referral to …. - issued by) who were not seen should be recorded in the daily report.
On the Director’s order (signal 2), all patients immediately leave the building.
Write the procedures for Director, Registration, Doctor, and Patient to simulate the operation of the clinic.
