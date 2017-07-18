LIBRARY()


NO_COMPILER_WARNINGS()

CFLAGS(-DSQLITE_THREADSAFE)
CFLAGS(-DSQLITE_OS_OTHER)
CFLAGS(-DSQLITE_SYSTEM_MALLOC)
CFLAGS(-DHAVE_USLEEP)

CFLAGS(-DSQLITE_INT64_TYPE=i64)
CFLAGS(-DSQLITE_UINT64_TYPE=ui64)
CFLAGS(-DUINT32_TYPE=ui32)
CFLAGS(-DUINT16_TYPE=ui16)
CFLAGS(-DINT16_TYPE=i16)
CFLAGS(-DUINT8_TYPE=ui8)
CFLAGS(-DINT8_TYPE=i8)

SRCS(
    # yandex specific sources
    os_yandex.cpp
    mutex_yandex.cpp

    # original sources
    sqlite3.c
)

END()