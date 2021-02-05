# AnR: PSPS Avionics and Recovery GitHub
This document was last edited by Jeffrey Kaji on *2 Jan 2021*. <br/>
View our license [here](LICENSE)

## MITCH_2021
**Multi-Instrument Telemetry Computer Hardware** for PSPS 2021 Rocket<br> 
Important Macros: <br>
``getBit(A, X)`` : gets the *"Xth"* bit of *"A"*. _(By VV)_<br>
``setBit(A, X, V)`` : sets the *"Xth"* bit of *"A"* to *"V"*. _(By VV)_ <br><br>
Debuging Macros: <br>
``print()`` : Prints to console. Is disabled by ``NDEBUG``. Does not accept string formatters. _(By JK)_ <br>
``prints()`` : Prints to console. Is disabled by ``NDEBUG``. Requires string formatters. _(By JK)_ <br>
``printe()`` : Prints to error stream. Is disabled by ``NDEBUG``. Requires string formatters. _(By JK)_ <br>

#### Quick Links
* [Documentation Folder **Docs/**](../Docs/README.md)
* [**Recovery-Avionics Jira**](https://psps-recovery-avionics.atlassian.net/jira/software/projects/AV/boards/2)
* [**SRAD Software Confluence Page**](https://psps-recovery-avionics.atlassian.net/wiki/spaces/PSPS/pages/31457281/SRAD+Software)
* [**Task Flowcharts Google Drive Folder**](https://drive.google.com/drive/u/0/folders/1KGmiUlNgTaysgdSle6QIzn_O4cT77nek)
* [**Hardware Datasheets Google Drive Folder**](https://drive.google.com/drive/u/0/folders/14jC0XhoOVIF4Ko5ylgHbCWUhaxgak8n0)

---
**Creating a *README.md*?** <br />
 Try using this free online editor that makes it super easy: 
<https://jbt.github.io/markdown-editor/>
