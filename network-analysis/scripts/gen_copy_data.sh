#! /bin/bash

root=~/code/eebo

dataset=$1
output_dir=$root/network-analysis/$dataset

host=shell.cs.helsinki.fi
host_dir=/cs/home/hxiao/public_html/eebo/$dataset
name1=author_network_of_copying_relation.graphml
name2=text_network_of_copying_relation.graphml

ssh="ssh ${host}"

echo "generating data frames"
python copying_info_dataframe.py \
    --path $root/eebo-tcp-similarity/$dataset/results.xml \
    --output_dir $output_dir

echo "building author network(using author_short)"
python copying_author_network.py \
    --cp_df_path $output_dir/copying_dataframe.pkl \
    --txt_df_path $output_dir/text_dataframe.pkl \
    --output_dir $output_dir

echo "building author network(using author_long)"
python copying_author_network.py \
    --cp_df_path $output_dir/copying_dataframe.pkl \
    --txt_df_path $output_dir/text_dataframe.pkl \
    --use_author_long \
    --output_dir $output_dir


echo "building text network"
python copying_text_network.py \
    --cp_df_path $output_dir/copying_dataframe.pkl \
    --txt_df_path $output_dir/text_dataframe.pkl \
    --output_dir $output_dir

${ssh} rm -rf ${host_dir}
scp -r $output_dir ${host}:${host_dir}
${ssh} chmod -R a+rx ${host_dir}/
