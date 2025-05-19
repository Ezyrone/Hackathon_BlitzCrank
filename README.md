# Hackathon_BlitzCrank# 🤖 Mini-Sumo Bot – Hackathon M1 2025

Bienvenue sur le dépôt officiel de notre robot mini-sumo !  
Ce projet a été réalisé dans le cadre du **Hackathon M1 2025** organisé à Grenoble.  
L’objectif : **construire, programmer et faire combattre un robot autonome capable de pousser ses adversaires hors d’un cercle.**

---

## 🧑‍💻 Équipe

**Nom de l’équipe** : _BlitzCrank_ 
**Membres** :
- Mehdi CHEDAD (AL)
- Kamil ZIDI (IW)
- Jory GRZESZCZAK (AL)

---

## ⚙️ Matériel utilisé

- 1 kit de robot à roues omnidirectionnelles ([voir le kit](https://fr.aliexpress.com/item/1005002486315926.html))
- 2x contrôleurs moteurs L298N
- 4x accus 3.7V / 3400mAh
- 1x Arduino UNO (contrôle bas niveau)
- 1x Raspberry Pi Zero (intelligence)
- 1x capteur ultrason HC-SR04
- 1x capteur infrarouge de ligne
- Divers outils fournis par l’organisation

---

## 🧠 Architecture logicielle

- **Arduino UNO**
  - Contrôle des moteurs via les L298N
  - Lecture des capteurs
  - Communication série avec le Raspberry Pi

- **Raspberry Pi Zero (Python)**
  - Prise de décision
  - Analyse capteurs
  - Envoi d’ordres moteurs à l’Arduino via `pyserial`

---



