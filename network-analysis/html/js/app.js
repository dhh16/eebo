var width = 1200,
    height = 800;

var text_box_width=400,
    text_box_height=100;

var min_node_size=5,
    max_node_size=15;

var color = d3.scale.category20();

var force = d3.layout.force()
    .charge(-120)
    .linkDistance(30)
    .size([width, height]);

var svg = d3.select("body").append("svg")
    .attr("width", width)
    .attr("height", height);

d3.json("graph.json", function(error, graph) {
    if (error) throw error;
    
    var pagerank_scores = _.map(graph.nodes, function(n){return n.pageranks;});
    var node_size = d3.scale.linear()
	.domain([_.min(pagerank_scores), _.max(pagerank_scores)])
	.range([min_node_size, max_node_size]);
    force
	.nodes(graph.nodes)
	.links(graph.links)
	.start();

    var link = svg.selectAll(".link")
	.data(graph.links)
	.enter().append("line")
	.attr("class", "link")
	.style("stroke-width", 1);

    var node = svg.selectAll(".node")
	.data(graph.nodes)
	.enter().append("circle")
	.attr("class", "node")
	.attr("r", function(d){
	    return node_size(d.pageranks);
	})
	.style("fill", function(d) { return color(d.modularity_class); })
	.on('click', function(d){
	    var text = node_labels.filter(function(other){
		return other['id'] == d['id'];
	    });
	    if (text.style('display') == 'none'){
		text.style('display', 'inline');
	    }
	    else{
		text.style('display', 'none');
	    }
	})
	.call(force.drag);

    var node_labels = svg.selectAll('foreignObject')
    	.data(graph.nodes)
    	.enter()
	.append('foreignObject')
    	.style('display', 'none')
	.attr('width', text_box_width)
    	.attr('height', text_box_height)
    	.on('click', function(d){
	    var me = d3.select(this);
	    me.style('display', 'none');
	});
    
    node_labels.append('xhtml:p')
	.text(function(d){return d.name;})


    node.append("title")
	.text(function(d) {
	    return d.name;
	});

    force.on("tick", function() {
	link.attr("x1", function(d) { return d.source.x; })
            .attr("y1", function(d) { return d.source.y; })
            .attr("x2", function(d) { return d.target.x; })
            .attr("y2", function(d) { return d.target.y; });

	node.attr("cx", function(d) { return d.x; })
            .attr("cy", function(d) { return d.y; });

	node_labels
	    .attr("x", function(d) { return d.x - text_box_width / 2; })
            .attr("y", function(d) { return d.y - text_box_height / 2; });
	    // .attr('transform', function(d){
	    // 	return 'translate(' + [d.x, d.y] + ')'; 
	    // });
    });   
});

