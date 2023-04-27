Diese Repository ist Teil des ct-Make Artikels "KI auf dem ESP32 - Teil 3", welcher in der Ausgabe 2/22 erschienen ist.

Es enthält den Beispielcode für das zweite Beispiel in welchem auch die Verwendung des ESP32CAM Moduls als Bildquelle für die neuronale Erkennung enthält.

Das Beispiel wurde unter ESP-IDF Version 4.3 und Version 4.4 getestet.

Sollte eine neuere Version verwendet werden und die Konfiguration in  `sdkconfig` angepasst werden, ist es wichtig folgende Parameter einzustellen:

| Parameter                               | Wert                       | Grund / Ursache                                              |
| --------------------------------------- | -------------------------- | ------------------------------------------------------------ |
| **Main task** stack size                | 9000                       | Erhöhung stack size, um genug Speicher für Bildverarbeitun zu haben |
| Support for external, SPI-connected RAM | aktivieren                 | Muss aktiv sein, damit PSRAM verwendet werden kann           |
| Partition Table                         | Custom partition table CSV | Verwendung einer manuelle Partitionierung, um die große Firmware flashen und ansprechen zu können |

 

#### Change log

##### 1.0.0 - Initiale Version (2022-04-01)

* Initiale Version zur Artikelveröffentlichung