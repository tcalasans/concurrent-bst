cd paper-version
bash test.sh
cd ..

cd transactional-version
bash test.sh
cd ..

rm -r output-dir
mkdir output-dir
cp ./paper-version/paper-output.csv ./output-dir
cp ./transactional-version/transaction-output.csv ./output-dir

zip -r output.zip ./output-dir
rm -r ./output-dir