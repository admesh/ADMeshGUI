BREW_PREFIX = $$system(brew --prefix 2> /dev/null)
!isEmpty(BREW_PREFIX){
    exists($$BREW_PREFIX/opt/admesh/lib/*){
        QMAKE_CXXFLAGS += -I/usr/local/opt/admesh/include
        LIBS += -L/usr/local/opt/admesh/lib
    }
    exists($$BREW_PREFIX/opt/stlsplit/lib/*){
        QMAKE_CXXFLAGS += -I/usr/local/opt/stlsplit/include
        LIBS += -L/usr/local/opt/stlsplit/lib
    }
    exists($$BREW_PREFIX/opt/gettext/lib/*){
        QMAKE_CXXFLAGS += -I/usr/local/opt/gettext/include
        LIBS += -L/usr/local/opt/gettext/lib
    }
}
