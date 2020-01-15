#-------------------------------------------------------------------------------
# Name:        read_config.py
# Purpose:     parser program data from configuration file
#-------------------------------------------------------------------------------

import sys # модуль для приёма программой входных параметров (argv) при запуске
import configparser # модуль для распарсивания данных из конфигурационных файлов

def has_ParamOption(config, section, option):
    if not config.has_option(section, option):
        # вывод на экран сообщения об отсутствии данного параметра задачи
        print("There is not parameter " + option + " in config file!")
        return False
    return True

def has_Params(config, section, options):
    # в случае, если в секции конфиг-файла содержатся ещё какие-нибудь опции кроме обязательных, они пропустятся
    retval = True
    for i in range(len(options)):
        if not has_ParamOption(config, section, options[i]):
            retval = False
    return retval

def get_ListNecessaryOptions():
    options = ['dimension', 'wave_type', 'computing_method', 'points_source', 'd', 'alpha', 'beta', 'h', 'max_iter', 'eps']
    return options

def get_ListAuxOptions(points_source):
    options = []
    if points_source == 'generate_grid':
        options = ['discr', 'steps', 'point_coordinates']
    if points_source == 'generate_random':
        options = ['discr', 'steps', 'sample', 'point_coordinates']
    return options

def parse_Params(config, section, options):
    str_params = ''
    for i in range(len(options)):
        if (config.has_option(section, options[i])):
            results = config[section][options[i]].split()
            j = 0
            while True:
                if options[i] == 'wave_type' or options[i] == 'computing_method' or options[i] == 'points_source':
                    str_params += str(config[section][options[i]]).upper()+"\n"
                    break
                if j >= len(results):
                    break
                else:
                    str_params += results[j]+"\n"
                    j+=1
    return str_params

def parser_ConfigData(configfile, section):
    config = configparser.ConfigParser()
    config.read(configfile)
    necessary_options = get_ListNecessaryOptions()
    if not has_Params(config, section, necessary_options):
        print("Error! Not all input task parameters are contained in config file!")
        return
    aux_options = get_ListAuxOptions(config[section]['points_source'])
    if not has_Params(config, section, aux_options):
        print("Error! Not all input task parameters are contained in config file!")
        return
    options = necessary_options + aux_options
    str_params = parse_Params(config, section, options)
    # распечатка распарсенных данных во вспомогательный файл
    f = open("in.txt",'w')
    f.write(str_params)
    print("Parsing config data are COMPLETED!\n") # сообщение об успешном выполнении
    f.close()


if len(sys.argv) != 3:
    print("Error! Script must be run in the format:\n python parser.py configfile readsection\n")
else:
    configfile = str(sys.argv[1]) # конфигурационный файл, из которого будет производиться считывание данных
    reading_section = str(sys.argv[2]) # секция конфигурационного файла, по которой будет определяться группа параметров, которую требуется считать
    parser_ConfigData(configfile, reading_section)