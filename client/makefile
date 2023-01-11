# ����ѥ���ʤɤ����ܤ���Ƥ��ޤ��󤬡�
# make�Υǥե����ư��(���ۤΥ롼��)��ȤäƤ��ޤ���
# make���ɤ�����ư��򤹤�Τ���-n���ץ����ǳ�ǧ�Ǥ��ޤ���
# �㡧 make -n �������å�
#
CC = g++
TEX = platex
CFLAGS = -Wall $(COPTIONS)
LDFLAGS = 
LOADLIBES = 
LDLIBS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lglut -lGLU -lGL -ljoyconlib -lhidapi-hidraw -lSDL2_gfx -ljoyconlib -lopenal -lalut -lhidapi-hidraw -lXext -lX11 -lXmu -DWIN32

TARGET = SB_client
SRCS = client_main.cpp client_command.cpp client_net.cpp client_win.cpp client_func.cpp glm.cpp gltx.cpp trackball.cpp
HEADS = client.h glm.h gltx.h trackball.h

# ����˼���Ȥ�ˤϤ��Υ������åȤ�
#   -g3 �ϥǥХå�����������४�ץ����
#
# �������åȻ��ꤷ�ʤ��ȡ��ǽ�Υ������åȤǤ��뤳�줬���Ф��
#
debug: CPPFLAGS = 
debug: COPTIONS = -g3
debug: $(TARGET)

# �ǥХå�����������ޤʤ��������å�
#   -O3 �Ϻ�Ŭ�����ץ����
#
# ������θĿͳ�ȯ�ʤɤǡ������ʤ�make����ݤʤɤ˻Ȥ��Ȥ褤
#
release: CPPFLAGS = -DNDEBUG
release: COPTIONS = -O3 
release: $(TARGET)

# ���ۤΥ롼��Ǥ�ư��ϰʲ����̤�
# c���������饪�֥�������
#   $(CC) $(CFLAGS) $(CPPFLAGS) -c n.c -o n.o
# ���֥������Ȥ���¹ԥե�����
#   $(CC) $(LDFLAGS) n.o $(LOADLIBES) $(LDLIBS) -o n
#
$(TARGET): $(SRCS)
	$(CC) $(SRCS) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) -o $(TARGET)

# ��ݡ��Ⱥ����ѥ������å�
# �㤨�� repo.tex ������Ȥ���
#  make repo.pdf ��pdf�ޤ���������
#
# tex����dvi�ؤϰ��ۤΥ롼�� $(TEX) n.tex ���Ȥ���
#
%.pdf:%.dvi
	dvipdfmx $<

# ����ѥ������ľ���������ʤɤˡ�
# ��֥ե�����ʤɤ������륿�����å�
#   �����㡧 make clean
#
.PHONY: clean
clean:
	@rm -f $(TARGET) $(OBJS) core *~
