# Mobile Applikasjoner - Arbeidskrav

## Note.io


## Oppgavebeskrivelse

### Tverrspråkelig Prosjektoppgave - Avansert Qt-Applikasjon

- Mål
  
  Studentene skal utvikle en applikasjon for hverdagsbruk (f.eks handleliste, oppgaveplanlegger, budsjett-app, treningslogg, notatbok).

  Prosjektet skal utvikles over flere uker, og skal inkludere avanserte funksjoner
  slik at sluttproduktet blir en profesjonell applikasjon.

  Studentene får velge følgende programmeringsspråk:

  1. C++ (Qt Widgets)
  2. Python (PyQt5 / PySide6)
  3. Java (Qt Jambi)
  4. etc...

- Minimumskrav (grunnlag)

  1. **GUI med Qt Widgets**
     1. Bruk QMainWindow eller QWidget med knapper, tekstfelst, tabeller/lister.
     2. Bruk Layouts (QVBoxLayout, QHBoxLayout, QGridLayout)
  2. **Data håndtering**
     1. Bruk fil (txt/csv/json) eller SQLite-database
     2. Implementer CRUD (create, read, update, delete).
  3. **Interaksjon**
     1. Bruk signals/slots eller tilsvarende (knappetrykk -> handling)
     2. Minst én dialog (QMessageBox, QFileDialog, etc...).
  4. **Funksjonalitet**
     1. Brukeren skal kunne legge inn, endre og slette data.
     2. Data skal kunne vises oversiktiglig (liste/tabell).

- Utvidede krav (avansert nivå)
  
1. **Database**
   1. Migrer fra enkel fil til SQLite (via QSqlDatabase og QSqlTableModel)
   2. Implenenter søk og filtrering
   3. Støtt import/eksport (csv, json, xml).
2. **GUI og brukeropplevelse**
   1. Flere vinduer eller faner (QTabWidget)
   2. Innstillinger/Preferanser lagret i QSettings
   3. Tema (lys/mørk) modus med Qt Style Sheets (QSS)
   4. Dra-og-slipp og kontekst menyer
3. **Statistikk og visualisering**
   1. Vis grafer (QtCharts, Matplotlib Python, etc.)
   2. Statistikk: summeringer, fordelinger og tidslinjer.
4. **Avansert funksjonalitet**
   1. Timer/varsling med QTimer.
   2. Multithreading for tunge operasoner (QThread / QtConcurrent)
   3. Nettverk (REST API via QNetworkAccessManager - f.eks synkronisering med server)
5. **Brukerstøtte**
   1. Innlogging med brukere/roller.
   2. Egne data for hver bruker.
6. **Profesjonelle krav**
   1. Dokumentasjon (README/manual)
   2. Versjonskontroll med Git.
   3. Enhetstesting (QTest i C++, pytest-qt i Python og JUnit i Java).


### Planlegging:

Lage en gjøreliste app. Kalles Note.io.

Den skal inneholde:

1. Grafisk bruker grensesnitt
2. Database for brukere og gjørelister
3. Innlogging
4. Representere gjørelister på en ryddig måte
5. Kunne redigere, slette og legge til gjørelister
6. 

Brukeren skal kunne opprette nye, redigere eksisterende og slette gjørelister.
Gjørelistene skal struktureres i sidemeny slik at de enkelt kan velges og redigeres.

Det skal også være en hoved side hvor gjøremålene vises med frist. Filtrert med nærmeste frist først.

## Notater

- bgColor, textColor skulle hatt disse på 1 sted. (QSS)

### Huskeliste:

1. Kryptere passord
2. Rette på new account knapp.

### Logg:
- 19.09.25
  Ferdigstilte klassediagrammet, og tenker å lage use case diagrammer og flyt skjema.

- 18.09.25
  Satte i gang med dokumentasjon og tegner opp klasse diagram.
  Føre opp notater og bemerkninger.

- Periode: 05.09.25 - 09.09.25
  Gjorde meg kjent med strukturen i programmet,
  hvor jeg har rotet meg litt bort i forhold til sammenhengen mellom sidemenu og maincontent.
  
- 04.09.25 Torsdag:
  Satte opp kryptering av passord, slik at passord ikke lagres i klartekst.

- 03.09.25 Onsdag:
  Satte opp logikk for knappene i TodoPage og begynte å ordne Home layout.

- 01.09.25 Mandag:
  Importere database oppføringene til de respektive klassene ved oppstart av programmet.

- 31.08.25 Søndag:
  Lage todo klasse og funksjonalitet

- 29.08.25 Fredag:
  Opprette klasser for database tabeller

- 27.08.25 Onsdag:

Splittet applikasjonen opp i flere filer for å øke detaljnivået og lesbarhet.

- 26.08.25 Tirsdag:

Satt opp grunnlaget for applikasjonen og opprettet Github repository.

