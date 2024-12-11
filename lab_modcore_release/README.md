Требуется подготовить модуль для загрузки в ядро Linux. Модуль должен:

    сопровождаться Makefile для сборки;
    оформляться в виде файла *.ko;
    загружаться командой insmod и выгружаться командой rmmod;
    при загрузке выводить в dmesg строку «Welcome to the Tomsk State University»;
    при выгрузке выводить в dmesg строку «Tomsk State University forever!».

запускается через:

    make (собрать модуль ядра по makefile)
    sudo dmesg -C (отчистить буфер ядра)
    sudo insmod mod_core1.ko (загрузить модуль)
    sudo rmmod mod_core1.ko (выгрузить модуль)
    sudo dmesg (прочитать буфер ядра)
