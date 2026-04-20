# ts-data-engine
Efficient Time-Series Data Storage Engine written in modern C++

History (to do: move this elsewhere)
-
Init
- Add basic ingest(), single event or batch, and query() implementation for StorageEngine --> single threaded for now. Data is kept in memory in a vector.
- Create a DataGenerator for random market type data. Generates between 1.3 to 1.4 million events/second (single threaded).
- Estimate performance in current state: Test query retrieves around 190.000 events in around 140ms => average per event retrieved around 750ns

Apr 20th 2026
- Separated the producer and consumer into different executable => now we can start them in different threads so that they won't share the same address space (as previous iteration was, hence the good speed). Order of start: storage_engine before data_generator
- Other minor changes
- New performance estimates for data transfer/ingestion: DataGenerator makes and puts into socket around 0.5 million events/second (TO DO: check if putting larger chunks into the socket will speed things up, compared to 1 produced => 1 sent).
- The overhead that the consumer adds to the system (receive data and store to internal vector) is below 1ms (usually below 0.5ms with some spikes above that).
- Querying time is the same since that part of the system did not change.

Next steps:
- [X] Move the Data Generation and Storage Engine on different threads
- [X] Add communication mechanism between them (socket)
- [X] Keep them single threaded for now
- [ ] Add disk (persistent) storage => this will move the pressure from memory to disk which should allow to conduct longer performance tests without worrying about RAM memory.