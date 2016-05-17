#! /bin/bash

root=/cs/home/hxiao/code/eebo

dataset=$1
output_dir=$root/network-analysis/$dataset

host=shell.cs.helsinki.fi
host_dir=/cs/home/hxiao/public_html/eebo/$dataset
name=author_network_of_copying_relation.graphml

echo "generating data frames"
python copying_info_dataframe.py \
    --path $root/eebo-tcp-similarity/$dataset/results.xml \
    --output_dir $output_dir

echo "building author network"
python copying_author_network.py \
    --cp_df_path $output_dir/copying_dataframe.pkl \
    --txt_df_path $output_dir/text_dataframe.pkl \
    --output_dir $output_dir


ssh ${host} mkdir -p ${host_dir}
scp $output_dir/${name} ${host}:${host_dir}
ssh ${host} chmod -R a+rx ${host_dir}/