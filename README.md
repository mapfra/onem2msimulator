# oneM2M-simulator

## Getting started with OMNeT++

The oneM2M simulator has been developed under the OMNeT++ environnement. 
To experiment this code you need : 
- to download the last version of the OMNeT++ environment from https://OMNeTpp.org/download/
- to install the IDE on your computer according to the ionstallation guide https://doc.OMNeTpp.org/OMNeTpp/InstallGuide.pdf

Once OMNeT++ is installed and tested you can upload the oneM2M simulator: 

- download the source code of the oneM2M simulator :
```
cd existing_repo
git remote add origin https://labs.etsi.org/rep/iot/smartm2m-onem2m-performance-evaluation/onem2m-simulator.git
git branch -M main
git push -uf origin main
```
or 
    download the project archive [oneM2M-simulator.zip](https://labs.etsi.org/rep/iot/smartm2m-onem2m-performance-evaluation/onem2m-profiler/-/archive/main/onem2m-profiler-main.zip)

## Getting stated with the oneM2M simulator 
- On your computer launch OMNeT++
    - create and select the workspace OMNeT++
    - on the oment++ menu bar -> File -> Import -> Projects from Folder or Archive 
    - Next>
    - Import source -> Archive 
    - Select the archive oneM2M-simulator.zip
    - only select oneM2M-simulator

At this stage the project is loaded on the IDE. 

- How to run the simulator : 
    - right click on onem2m-simulator 
    - run as OMNeT++ Simulation

    
## Configuring a new simulation
In order to define a new scenario, you will need to follow this 2 steps
 
### Step 1 Define the topology of new oneM2M-based IoT Solution:

Create a new network (.ned file) : This file describes the topology of the system. 

You can edit the file in text mode or use the GUI editor. In GUI mode, all simulaiton modules are available on the left panel (modules), that can be placed (drag-and-drop) on the simulation. 

For information on modules and how to connect them, please refer to these documents :
 
1. Definition of the meta-model of an IoT Solution based on oneM2M in [ETSI TS 103840 SmartM2M; Model for oneM2M Performance Evaluation](https://www.etsi.org/deliver/etsi_ts/103800_103899/103840/01.01.01_60/ts_103840v010101p.pdf)

2. Description of the simulation modules used to implement the meta-model in [ETSI TR 103841 SmartM2M; oneM2M Performances Evaluation Tool (Proof of Concept)](https://portal.etsi.org/eWPM/index.html#/schedule?WKI_ID=63605)

### Step 2 Configure the simulation modules

Create a new configuration file (.ini) : this file provision values for the simulaiton modules' parameters. 

Other information related to the simulaiton process can be added here such as number or runs, random number generators, etc.

For more information about how to configure an OMNeT++ simulation, please refer to OMNeT++ manual.  
