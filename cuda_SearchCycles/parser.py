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


def has_AllTaskParamsAtSection(config, section):
	# формируется список всех обязательных входных параметров задачи
    # в случае, если в секции конфигурационного файла содержатся ещё какие-нибудь опции кроме обязательных, они пропустятся
    options = ['dimension', 'initset_proections', 'points_source', 'discr', 'skip_iter', 'max_iter', 'max_cycle_len', 'checks', 'eps', 'tau', 'name_math_mapping']
    retval = True
    for i in range(len(options)):
        if not has_ParamOption(config, section, options[i]):
            retval = False
    return retval


def read_AuxMathMappingParams(config, section, option):
    if (config.has_option(section, 'math_mapping_params')):
        return config[section][option].split()
    return []

def parser_ConfigData(configfile, section):
    config = configparser.ConfigParser()
    config.read(configfile)
    if not has_AllTaskParamsAtSection(config, section):
        print("Error! Not all input task parameters are contained in config file!")
        return
    dimension = config[section]['dimension']
    initset_proections = config[section]['initset_proections']
    points_source = str(config[section]['points_source']).strip().upper()
    discr = config[section]['discr']
    skip_iter = config[section]['skip_iter']
    max_iter = config[section]['max_iter']
    max_cycle_len = config[section]['max_cycle_len']
    checks = config[section]['checks']
    eps = config[section]['eps']
    tau = config[section]['tau']
    name_math_mapping = str(config[section]['name_math_mapping']).strip().upper()
    math_mapping_params = read_AuxMathMappingParams(config, section, 'math_mapping_params') # необязательная опция для чтения из конфигурационного файла

    # распечатка распарсенных данных во вспомогательный файл
    f = open("input.txt",'w')
    parsedata_str = dimension+"\n"
    f.write(parsedata_str)
    results = initset_proections.split()
    i = 0
    while True:
        if i >= len(results):
            break
        else:
            parsedata_str = results[i]+" "+results[i+1]+"\n"
            f.write(parsedata_str)
            i+=2
    parsedata_str = points_source+"\n"+discr+"\n"+skip_iter+" "+max_iter+"\n"+max_cycle_len+"\n"+checks+"\n"+eps+" "+tau+"\n"+name_math_mapping+"\n"
    f.write(parsedata_str)
    parsedata_str = str(len(math_mapping_params))+"\n"
    f.write(parsedata_str)
    i = 0
    while i<len(math_mapping_params):
        f.write(math_mapping_params[i]+" ")
        i+=1
    print("Parsing config data are COMPLETED!") # сообщение об успешном выполнении
    f.close()


if len(sys.argv) != 3:
    print("Error! Script must be run in the format:\n python parser.py configfile readsection\n")
else:
    configfile = str(sys.argv[1]) # конфигурационный файл, из которого будет производиться считывание данных
    section = str(sys.argv[2]) # секция конфигурационного файла, по которой будет определяться группа параметров, которую требуется считать
    parser_ConfigData(configfile, section)