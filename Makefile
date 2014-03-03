TARGET = current

GITBINARY := git
FEHURL := google.com
FIRMWAREREPO := fehproteusfirmware

all:
ifeq ($(OS),Windows_NT)	
# check for internet connection
# if there's internet, check to see if FEHRobotController folder exists
# if it does, remove it before cloning the repo
	@ping -n 1 -w 1000 $(FEHURL) > NUL & \
	if errorlevel 1 \
	( \
		( echo "Warning: No internet connection!" ) \
	) \
	else \
	( \
		( if exist "$(FIRMWAREREPO)" \
		( \
			rd /s /q $(FIRMWAREREPO) \
		) ) && \
		$(GITBINARY) config --global http.sslVerify false  && \
		$(GITBINARY) clone https://redmine.engr1.ohio-state.edu/gitlab/feh/$(FIRMWAREREPO).git \
	)
else
# Linux
	if [ -d $(FIRMWAREREPO) ]; then \
                $(GITBINARY) --git-dir=$(FIRMWAREREPO)/.git pull ; \
	else \
                $(GITBINARY) clone https://redmine.engr1.ohio-state.edu/gitlab/feh/$(FIRMWAREREPO).git ; \
	fi

endif
	@cd $(FIRMWAREREPO) && make all TARGET=$(TARGET)

deploy:
	@cd $(FIRMWAREREPO) && make deploy TARGET=$(TARGET)

clean:
	@cd $(FIRMWAREREPO) && make clean TARGET=$(TARGET)

run:
	@cd $(FIRMWAREREPO) && make run TARGET=$(TARGET)
