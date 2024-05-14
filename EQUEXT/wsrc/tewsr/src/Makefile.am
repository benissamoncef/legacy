prgbindir=$(prefix)/bin
prgbin_PROGRAMS = tewsr

tewsr_SOURCES = tewsr.c \
		   option.c \
		   util.c \
		   stub.c \
		   minini.c

tewsr_LDADD   = $(LDFLAGS) \
                    -L$(WSFC_HOME)/lib \
                    -L../../../../XDMICG/libx \
                    ../../../../XDMICG/libx/xzs.a \
                    ../../../../XDMICG/libx/xzaoa.a \
                    ../../../../XDMICG/libx/xd.a \
                    ../../../../XDMICG/libx/xzc.a \
                    ../../../../XDMICG/libx/xzd.a \
                    ../../../../XDMICG/libx/xze.a \
                    ../../../../XDMICG/libx/xzeh.a \
                    ../../../../XDMICG/libx/xzi.a \
                    ../../../../XDMICG/libx/xzm.a \
		     		../../axis2c/util/src/libaxutil.la \
					../../axis2c/axiom/src/om/libaxis2_axiom.la \
					../../axis2c/src/core/engine/libaxis2_engine.la \
                    -lpthread \
					../../axis2c/src/core/transport/http/sender/libaxis2_http_sender.la \
					../../axis2c/src/core/transport/http/receiver/libaxis2_http_receiver.la \
					../../axis2c/neethi/src/util/libneethi_util.la \
					../../axis2c/neethi/src/libneethi.la \
					$(GUTHTHILA_LIBS) \
					$(RAMPART_LIBS) \
					$(OMXMLSEC_LIBS) \
					$(SECCONV_LIBS) \
                    $(LIBXML2_LIBS)

INCLUDES =	-I ../../../../XDMICG/inc \
            -I ../../axis2c/util/include \
            -I ../../axis2c/include \
            -I ../../axis2c/axiom/include \
            -I ../../axis2c/neethi/include \
			-I ../../axis2c/neethi/src/util \
            -I ../../rampartc/include
            

EXTRA_DIST=constants.h error.h option.h stub.h util.h tewsr.h


