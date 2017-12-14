
service apache2 start
service mysql start
echo "create database srmcollider" | mysql

wget https://svn.code.sf.net/p/sashimi/code/tags/release_4-3-1/trans_proteomic_pipeline/perl/SSRCalc3.par
wget https://svn.code.sf.net/p/sashimi/code/tags/release_4-3-1/trans_proteomic_pipeline/perl/SSRCalc3.pl

wget http://www.peptideatlas.org/builds/human/201111/APD_Hs_all.fasta
grep -c ">" APD_Hs_all.fasta 
grep -v ">" APD_Hs_all.fasta > APD_Hs_peptides.fasta
perl /home/srmcollider/ssrcalc/SSRCalc3.pl --alg 3.0 --source_file APD_Hs_peptides.fasta  --output tsv --B 1 --A 0  > APD_Hs_peptides.ssrcalc
grep -v Z APD_Hs_peptides.ssrcalc | grep -v B > APD_Hs_peptides.fix.ssrcalc
python create_db.py --mysql_config=/home/srmcollider/.srm.cnf --peptide_table=srmcollider.srmPeptides_human_PeptideAtlas --tsv_file=APD_Hs_peptides.fix.ssrcalc


