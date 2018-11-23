"""
===========================================
FeatureHasher and DictVectorizer Comparison
===========================================

text documents.

The example demonstrates syntax and speed only; it doesn't actually do
anything useful with the extracted vectors. See the example scripts
{document_classification_20newsgroups,clustering}.py for actual learning
on text documents.
A discrepancy between the number of terms reported for DictVectorizer and
for FeatureHasher is to be expected due to hash collisions.
"""

# Author: Lars Buitinck <L.J.Buitinck@uva.nl>
# License: BSD 3 clause
from __future__ import print_function
from collections import defaultdict
import re
import sys
from time import time


from sklearn.datasets import fetch_20newsgroups
from sklearn.feature_extraction import DictVectorizer, FeatureHasher


def n_nonzero_columns(X):
return len(np.unique(X.nonzero()[1]))


def tokens(doc):
"""Extract tokens from doc.

principled approach, see CountVectorizer or TfidfVectorizer.
"""
return (tok.lower() for tok in re.findall(r"w+", doc))


def token_freqs(doc):
freq = defaultdict(int)
for tok in tokens(doc):
freq[tok] += 1
return freq


'alt.atheism',
'comp.graphics',
'comp.sys.ibm.pc.hardware',
'misc.forsale',
'rec.autos',
'sci.space',
]
# Uncomment the following line to use a larger set (11k+ documents)
#categories = None

print(__doc__)
print("Usage: %s [n_features_for_hashing]" % sys.argv[0])
print()

try:
n_features = int(sys.argv[1])
except IndexError:
n_features = 2 a aa.cc aa.rs a.out BCH bch_codec charUchar.cc crc crc.cc filecRS find_last_of find_last_of.cc forTest forTest.cc genBCHnkt genBCHnkt.cc hashing_vs_dict_vectorizer.py ll Reed-Solomon regex regex.cc split.sh strRespace.cc text.py twoDvector twoDvector.cc writeTest.cc 18
print("not a valid number of features: %r" % sys.argv[1])
sys.exit(1)


print("Loading 20 newsgroups training data")
raw_data = fetch_20newsgroups(subset='train', categories=categories).data
print("%d documents - %0.3fMB" % (len(raw_data), data_size_mb))
print()

print("DictVectorizer")
t0 = time()
vectorizer = DictVectorizer()
duration = time() - t0
print("done in %fs at %0.3fMB/s" % (duration, data_size_mb / duration))
print("Found %d unique terms" % len(vectorizer.get_feature_names()))
print()

print("FeatureHasher on frequency dicts")
hasher = FeatureHasher(n_features=n_features)
X = hasher.transform(token_freqs(d) for d in raw_data)
duration = time() - t0
print("done in %fs at %0.3fMB/s" % (duration, data_size_mb / duration))
print("Found %d unique terms" % n_nonzero_columns(X))
print()
print("FeatureHasher on raw tokens")
t0 = time()
hasher = FeatureHasher(n_features=n_features, input_type="string")
X = hasher.transform(tokens(d) for d in raw_data)
duration = time() - t0
print("done in %fs at %0.3fMB/s" % (duration, data_size_mb / duration))
