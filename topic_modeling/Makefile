remote_dir=/cs/home/hxiao/public_html/eebo
all:
	ssh department rm -rf ${remote_dir}/topic_civil_war
	scp -r vis/ department:${remote_dir}/topic_civil_war
	ssh department chmod -R a+rx ${remote_dir}/topic_civil_war
