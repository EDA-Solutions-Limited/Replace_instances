#  Replace_instances


<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#change-log">Change log</a></li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This script finds and replaces specified instances of a cell with new instances of another cell  
The script inspiration was a mapping over of a tcl script which was resource intensive when replacing alot of intances consuming staggering amounts of ram  
* The C++ script uses a dialog interface to ask the user which cell instances to replace and then loops over all the instances in the visible layout  
It then creates a generator for new cells ( To ensure T-Cells are replaced as well) and then replaces the cells  
The chrono function was used to time the execution of the replacement in comparison with the tcl script  
* The Tcl script requires manual input of the cell instances to be replaced into the script  
It also finds and replaces the specified instances  
Additionally, the RAM usage is monitored and when the set limit is reached, the completion is reported at 10% intervals to let the user know how much has been replaced before exexution was stoppped.  

### Built With

* C++
* Tcl
* Tanner L-Edit 


<!-- GETTING STARTED -->
## Getting Started

To get started with using this code, ensure you have Tanner tools installed with an open design.

### Prerequisites
The C++ compiler MinGW is bundled with tanner tools so no need to install any compilers

The following header files on top of the default ones added by L-Edit at default  are needed for L-Edit functions to work in the case of the C++ script.
* string.h
* ldata.h
* lcomp.h


### Installation

1. Open the project at **.insert local path of script here** with an editor. Preferrably [**vscode**](https://code.visualstudio.com/)
2. You could also clone the repo
   ```sh
   git clone https://github.com/EDA-Solutions-Limited/Replace_instances.git

<!-- MAKING CHANGES -->
## Usage

- Drag and drop the UPI macro into L-Edit and go to Tools -> Execute Script or simply the F5 hotkey 
- Alternatively, you could go to Tools -> Macro -> Load, navigate to the path of the script. Click on Run
- Ensure you have made a selection in the design for the objects you intend to grown on a specified layer. Otherwise the script will show a **no selection** error.


<!-- ROADMAP -->
## Roadmap


<!-- CONTRIBUTING -->
## Contributing

Contributions are what makes EDA solutions the ultimate dream team. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
6. If the changes made are much more awesome, then the request would be approved and merged into the main branch. 

<!-- CHANGE LOG -->
## Change log
### 25-Sept-2020 C++ script
* Added Dialog item boxes for input
* Added chrono function to measure execution time vs the same script written in tcl

<!-- LICENSE -->
## License
MIT

<!-- CONTACT -->
## Contact

support@eda-solutions.com

Project Link: [Replace_instances](https://github.com/EDA-Solutions-Limited/Replace_instances)
