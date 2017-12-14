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

WORKDIR /home/srmcollider/
# RUN git clone https://github.com/Roestlab/srmcollider.git
RUN git clone https://github.com/Roestlab/srmcollider.git && cd srmcollider && git checkout feature/webtool

WORKDIR /home/srmcollider/srmcollider/code/
RUN python setup.py build
RUN python setup.py install

# setup web scripts
RUN mkdir -p /var/webapps/srmcollider/
RUN mkdir -p /var/websites/srmcollider
RUN cp -r cgi-scripts /var/webapps/srmcollider/web_scripts/
RUN cp -r css /var/websites/srmcollider/stylesheets/
RUN mkdir -p /var/websites/srmcollider/documents/
RUN chown www-data:www-data /var/websites/srmcollider/documents/
RUN rm /etc/apache2/sites-enabled/000-default.conf
COPY apache_config /etc/apache2/sites-enabled/srmcollider.conf 
RUN a2enmod cgi

COPY collider_config.py /var/webapps/srmcollider/web_scripts/
COPY .srm.cnf /home/srmcollider/
# RUN mv /var/webapps/srmcollider/web_scripts/srmcollider.py /var/webapps/srmcollider/web_scripts/collider.py
RUN chown www-data:www-data /var/www
WORKDIR /home/srmcollider/


# Fix script
RUN sed -i '/exit/d' /home/srmcollider/srmcollider/code/scripts/misc/create_db.py

# Install SSRCalc
RUN mkdir ssrcalc
WORKDIR /home/srmcollider/ssrcalc
RUN wget https://svn.code.sf.net/p/sashimi/code/tags/release_4-3-1/trans_proteomic_pipeline/perl/SSRCalc3.par
RUN wget https://svn.code.sf.net/p/sashimi/code/tags/release_4-3-1/trans_proteomic_pipeline/perl/SSRCalc3.pl
WORKDIR /home/srmcollider/

WORKDIR /home/srmcollider/srmcollider/code/scripts/misc/
COPY load_human_pa.sh .
RUN bash load_human_pa.sh
WORKDIR /home/srmcollider/

# Get entry point
COPY entrypoint.sh /home/srmcollider/
RUN chmod a+x  /home/srmcollider/entrypoint.sh

