import sys
import timeit
from Indexer import Indexer

def get_extracted_features(lines):
    label_to_features = []
    feature_set = set()

    word_to_labels = {}
    tagset = set()

    for line in lines:
        features = line.strip().split(' ')
        label = features[0]
        features = features[1:]

        tagset.add(label)

        c = sorted(features)
        label_to_features.append((label,c))
        feature_set.update(features)
        for f in features:
            if f.startswith('w='):
                _,w = f.split('=',1)
                if not w in word_to_labels:
                    word_to_labels[w] = set()
                word_to_labels[w].add(label)
                break

    return label_to_features,feature_set,word_to_labels,tagset

def main(feature_fname, feature_vec_fname, feature_map_fname):
    print 'Reading features file...'
    with open(feature_fname,'r') as file:
        lines = [line.strip() for line in file]

    print 'Extracting Features...',
    start_time = timeit.default_timer()
    label_to_features,feature_set,word_to_labels,tagset = get_extracted_features(lines)
    print 'Done. Elapsed:',timeit.default_timer() - start_time

    
    feat_indexer = Indexer(feature_set)
    tag_indexer = Indexer(tagset)

    print 'Writing feature vector file...',
    start_time = timeit.default_timer()
    with open(feature_vec_fname,'w') as feature_vec_file:
        for l,c in label_to_features:
            s = str(tag_indexer.to_index(l))

            for feat in c:
                s += ' %d:1'%(feat_indexer.to_index(feat))
            s += '\n'
            feature_vec_file.write(s)
    print 'Done. Elapsed:',timeit.default_timer() - start_time

    tag_indexer.save('tagset_map')

    print 'Writing feature map...'
    feat_indexer.save(feature_map_fname)

    print 'Appending the pruning data...'
    # append the extra data - for pruning
    with open(feature_map_fname,"a") as fmap:
        fmap.write('\n%%%%%\n')
        for w in word_to_labels:
            s = '%s'%w
            for label in word_to_labels[w]:
                s += ' %s'%label
            s += '\n'
            fmap.write(s)
    print 'Done.'
 
if __name__ == '__main__':
    if len(sys.argv) <= 3:
        print 'Usage: ConvertFeatures.py <features_file> <feature_vecs_file> <feature_map_file>'
        exit()
    main(sys.argv[1],sys.argv[2],sys.argv[3])