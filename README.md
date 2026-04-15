# ts-data-engine
Efficient Time-Series Data Storage Engine written in modern C++

- Add basic ingest(), single event or batch, and query() implementation for StorageEngine --> single threaded for now. Data is kept in memory in a vector.
- Create a DataGenerator for random market type data. Generates between 1.3 to 1.4 million events/second (single threaded).
- Estimate performance in current state: Test query retrieves around 190.000 events in around 140ms => average per event retrieved around 750ns

Next steps:
- [ ] Move the Data Generation and Storage Engine on different threads
- [ ] Add communication mechanism between them (some sort of pipe)
- [ ] Keep them single threaded for now