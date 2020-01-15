#-------------------------------------------------------------------------------
# Name:        read_config.py
# Purpose:     read program data from configuration file
#-------------------------------------------------------------------------------

import configparser

def read_configdata():
    config = configparser.ConfigParser()
    config.read('data.config')
    size = config['DATA_PROGRAM']['size']
    set_proections = config['DATA_PROGRAM']['set_proections']
    d = config['DATA_PROGRAM']['d']
    N = config['DATA_PROGRAM']['N']
    M = config['DATA_PROGRAM']['M']
    K = config['DATA_PROGRAM']['K']
    T = config['DATA_PROGRAM']['T']
    eps = config['DATA_PROGRAM']['eps']
    tau = config['DATA_PROGRAM']['tau']

read_configdata()
