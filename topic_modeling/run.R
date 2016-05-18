
library('tm')
library('mallet')
library('LDAvis')
library('lda')

dir.name <- "/homeappl/home/hxiao/Downloads/eebo-tcp-1642-1651-2-24"
documents <- mallet.read.dir(dir.name)$text

stop_words <- stopwords("SMART")

                                        # pre-processing:
documents <- gsub("'", "", documents)  # remove apostrophes
documents <- gsub("[[:punct:]]", " ", documents)  # replace punctuation with space
documents <- gsub("[[:digit:]]", " ", documents)  # replace digit with space
documents <- gsub("ſ", "s", documents) 
documents <- gsub("[[:cntrl:]]", " ", documents)  # replace control characters with space
documents <- gsub("^[[:space:]]+", "", documents) # remove whitespace at beginning of documents
documents <- gsub("[[:space:]]+$", "", documents) # remove whitespace at end of documents
documents <- tolower(documents)  # force to lowercase

                                        # tokenize on space and output as a list:
doc.list <- strsplit(documents, "[[:space:]]+")

                                        # compute the table of terms:
term.table <- table(unlist(doc.list))
term.table <- sort(term.table, decreasing = TRUE)

                                        # remove terms that are stop words or occur fewer than 5 times:
del <- names(term.table) %in% stop_words | term.table < 5
term.table <- term.table[!del]
vocab <- names(term.table)

                                        # now put the documents into the format required by the lda package:
get.terms <- function(x) {
    index <- match(x, vocab)
    index <- index[!is.na(index)]
    rbind(as.integer(index - 1), as.integer(rep(1, length(index))))
}
documents <- lapply(doc.list, get.terms)

D <- length(documents)  # number of documents (2,000)
W <- length(vocab)  # number of terms in the vocab (14,568)
doc.length <- sapply(documents, function(x) sum(x[2, ]))  # number of tokens per document [312, 288, 170, 436, 291, ...]
N <- sum(doc.length)  # total number of tokens in the data (546,827)
term.frequency <- as.integer(term.table)  # frequencies of terms in the corpus [8939, 5544, 2411, 2410, 2143, ...]


K <- 20
G <- 20  # #iterations
alpha <- 0.02
eta <- 0.02

set.seed(357)
t1 <- Sys.time()
fit <- lda.collapsed.gibbs.sampler(documents = documents, K = K, vocab = vocab,
                                   num.iterations = G, alpha = alpha,
                                   eta = eta, initial = NULL, burnin = 0,
                                   compute.log.likelihood = TRUE,
                                   trace=10L)
t2 <- Sys.time()
t2 - t1  # about 24 minutes on laptop

theta <- t(apply(fit$document_sums + alpha, 2, function(x) x/sum(x)))
phi <- t(apply(t(fit$topics) + eta, 2, function(x) x/sum(x)))

vis.data <- list(phi = phi,
                 theta = theta,
                 doc.length = doc.length,
                 vocab = vocab,
                 term.frequency = term.frequency)

json <- createJSON(phi = vis.data$phi,
                   theta = vis.data$theta,
                   doc.length = vis.data$doc.length,
                   vocab = vis.data$vocab,
                   term.frequency = vis.data$term.frequency)

serVis(json, out.dir = 'vis', open.browser = FALSE)
