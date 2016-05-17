host=shell.cs.helsinki.fi
host_dir=/cs/home/hxiao/public_html

tnc=text_network_of_copying_relation.graphml
anc=author_network_of_copying_relation.graphml

all: text_network author_network
	echo "done"

text_network:
	scp output/${tnc} ${host}:${host_dir}
	ssh ${host} chmod a+r ${host_dir}/${tnc}

author_network:
	scp output/${anc} ${host}:${host_dir}
	ssh ${host} chmod a+r ${host_dir}/${anc}