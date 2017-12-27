FROM ubuntu:16.04

RUN apt-get -y update
RUN apt-get -y install python python-dev python-setuptools
RUN apt-get -y install g++ make 
RUN apt-get -y install python python-mysqldb python-biopython 
RUN apt-get -y install python-dev libcgal-dev libboost-python-dev
RUN apt-get -y install libboost-filesystem-dev libboost-test-dev libboost-system-dev
RUN apt-get -y install git
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install mysql-server
RUN apt-get -y install apache2 libapache2-mod-python
RUN apt-get -y install python-matplotlib python-numpy python-scipy python-matplotlib
RUN apt-get -y install vim
RUN apt-get -y install wget

# Download and install srmcollider
WORKDIR /home/srmcollider/
WORKDIR /home/srmcollider/
# RUN git clone https://github.com/Roestlab/srmcollider.git
RUN git clone https://github.com/Roestlab/srmcollider.git && cd srmcollider && git checkout feature/webtool

WORKDIR /home/srmcollider/srmcollider/code/
RUN python setup.py build
RUN python setup.py install

# Setup web scripts

# 1) copy to directories
RUN mkdir -p /var/webapps/srmcollider/
RUN mkdir -p /var/websites/srmcollider/
RUN cp -r cgi-scripts /var/webapps/srmcollider/web_scripts/
RUN cp -r css /var/websites/srmcollider/stylesheets/
RUN mkdir -p /var/websites/srmcollider/documents/

# 2) Prepare apache
RUN chown www-data:www-data /var/websites/srmcollider/documents/
RUN rm /etc/apache2/sites-enabled/000-default.conf
RUN cp deploy_docker/apache_config /etc/apache2/sites-enabled/srmcollider.conf
RUN a2enmod cgi

# 3) Prepare collider config and mysql config
RUN cp deploy_docker/collider_config.py /var/webapps/srmcollider/web_scripts/
RUN cp deploy_docker/.srm.cnf /home/srmcollider/
RUN chown www-data:www-data /var/www
WORKDIR /home/srmcollider/

# 4) Fix script
RUN sed -i '/exit/d' /home/srmcollider/srmcollider/code/scripts/misc/create_db.py

# 5) Install SSRCalc
RUN mkdir ssrcalc
WORKDIR /home/srmcollider/ssrcalc
RUN wget https://svn.code.sf.net/p/sashimi/code/tags/release_4-3-1/trans_proteomic_pipeline/perl/SSRCalc3.par
RUN wget https://svn.code.sf.net/p/sashimi/code/tags/release_4-3-1/trans_proteomic_pipeline/perl/SSRCalc3.pl
RUN chmod a+x /home/srmcollider/ssrcalc/*
WORKDIR /home/srmcollider/

# 6) Load sample data (human peptide atlas)
WORKDIR /home/srmcollider/srmcollider/code/scripts/misc/
RUN cp /home/srmcollider/srmcollider/code/deploy_docker/load_human_pa.sh .
RUN bash load_human_pa.sh
WORKDIR /home/srmcollider/

# 7) Set up entry point
COPY entrypoint.sh /home/srmcollider/
RUN chmod a+x /home/srmcollider/entrypoint.sh
ENTRYPOINT /home/srmcollider/entrypoint.sh

# sudo docker run -dit -p 8080:80 srmcollider


