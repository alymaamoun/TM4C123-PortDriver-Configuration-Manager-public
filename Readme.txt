PDCM V1.2 Release Notes:

  Features and updates:
                         1- Port Driver updated to be AUTOSAR with version 4.0.3
                         2- Clear pin button added to enable the user to delete a configured Pin
                         3- The Configuration will be saved in a directory choosen by the user this directory contains:
                                                                                                                        1-MCAL    folder
                                                                                                                        2-HAL     folder
                                                                                                                        3-Config  folder
                                                                                                                        4-Common  folder
                                                                                                                        5-App     folder
                          4- the application of this version will generate:
                                                                            1-  Port.c              in Mcal
                                                                            2-  Port.h              in Mcal/include
                                                                            3-  Port_reg.h          in config
                                                                            4-  Port_cfg.h          in config
                                                                            5-  Port_PBcfg.c        in config
                                                                            6-  Common_Macros.h     in common
                                                                            7-  Compiler.h          in common
                                                                            8-  Det.c               in common
                                                                            9-  Det.h               in common
                                                                            10- Platform.h          in common
                                                                            11- Std_Types.h         in common
                                                                            12- tm4c123gh6pm_reg.h  in common
                          5-Application file contains two functions, Mcal_init and Hal_init this two functions are used to initialize the drivers of the Mcal layer and Hal
                          layer , put your drivers initialization in these two functions
                          
                          6- the main file call that two functions before the while(1)

Bugs fixed:
              
              
                          missed initial value in the configuration script
                          
                          
                          
                         
How to use:
  
                          - After clicking ok for the alerting messages the program will start , read that messages carefully
                          - in the interface you will find a picture for the tiva c and next to the pins you will find push buttons
                          - those buttons represesnt the suitable pins 
                          - by clicking any push button next to the picture the combo boxes will load their values 
                          - for example if you pressed PB2 the combo box will load port B pin 2 and their options 
                          - when you choose your options for the selected pin click set pin and its button will turn to led as indication that these pin is set
                          - if you want to delete that pin after set just click on it and press clear pin and its button will turn to white again 
                          - after finishing the configuration just click finish and after clicking ok it will open file browser
                          - choose the directory you wan to save your project in and click save and it will generate the files
                          - Hope it's useful!
                          
   Contact:
          Email:    aly.mammoun18@gmail.com
          Whatsapp: +2001097855135
                          
