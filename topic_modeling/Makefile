remote_dir=/cs/home/hxiao/public_html/eebo
# dataset=civil_war
dataset=sixteen

all:
	ssh department rm -rf ${remote_dir}/topic_${dataset}
	scp -r vis/ department:${remote_dir}/topic_${dataset}
	ssh department chmod -R a+rx ${remote_dir}/topic_${dataset}
