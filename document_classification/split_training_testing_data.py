# coding: utf8

import os
import numpy as np
from glob import glob
from sklearn.preprocessing import LabelEncoder
from sklearn.feature_extraction.text import CountVectorizer, TfidfTransformer
from sklearn.naive_bayes import MultinomialNB
from sklearn.linear_model import LogisticRegression
from sklearn.pipeline import Pipeline

import codecs

from util import preprocess

def open(f):
    return codecs.open(f, 'r', 'utf8')


clf = Pipeline([('vect', CountVectorizer(stop_words='english')),
                ('tfidf', TfidfTransformer()),
                ('clf', LogisticRegression()),
])

corpus_dir = '/homeappl/home/hxiao/Downloads/eebo-tcp-1642-1651-2-24/'
groundtruth_path = '/homeappl/home/hxiao/code/eebo/document_classification/data/civil-war/'

train_docs = []
train_doc_ids = []
train_labels = []
test_docs = []

for p in glob(groundtruth_path + '*.txt'):
    label = os.path.basename(p).split('.')[0].replace('_ids', '')
    doc_ids = [l.strip() for l in open(p)]
    train_docs += [open(os.path.join(corpus_dir, doc_id + '.txt')).read()
                  for doc_id in doc_ids]

    train_labels += [label] * len(doc_ids)
    train_doc_ids += doc_ids

assert len(train_doc_ids) == len(set(train_doc_ids))
assert len(train_labels) == len(set(train_docs))

train_doc_ids = set(train_doc_ids)

X_test = []
print "#documents: {}".format(len(glob(corpus_dir + '*.txt')))

for p in glob(corpus_dir + '*.txt'):
    doc_id = os.path.basename(p).split('.')[0]
    if doc_id not in train_doc_ids:
        X_test.append(open(p).read())

X_train = train_docs
label = LabelEncoder().fit(train_labels)
Y_train = label.transform(train_labels)


# preprocessing
X_train = [preprocess(doc) for doc in X_train]
X_test = [preprocess(doc) for doc in X_test]


print "#X_train: {}, #X_test: {}".format(len(X_train), len(X_test))
clf.fit(X_train, Y_train)
Y_test = clf.predict_proba(X_test)

print

for l, i in zip(label.classes_, xrange(Y_test.shape[1])):
    proba = Y_test[:, i]
    print "{}: {}".format(l, len(np.nonzero(proba >= 0.5)[0]))


weights = clf.get_params()['clf'].coef_[0,:]
feat_names = clf.steps[0][1].get_feature_names()

def top_features(weights, K=10):
    top_pos_ids = np.argsort(weights[weights > 0])[::-1][:K]
    return ' '.join([feat_names[i] for i in top_pos_ids])
print 

print u"{}: {}".format(label.classes_[0], top_features(weights, 20))
print u"{}: {}".format(label.classes_[1], top_features(-weights, 20))
