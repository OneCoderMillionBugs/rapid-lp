import multiprocessing
import time
import threading


def threadfoo_1(shared_list):
    for i in range(10):
        log(shared_list, ('Thread #1 added: %d' % i))
        time.sleep(0.01)


def threadfoo_2(shared_list):
    for i in range(10):
        log(shared_list, ('Thread #2 added: %d' % i))
        time.sleep(0.01)


def log(shared_list, msg):
    shared_list.append(msg)


def write_logs(shared_list, listen_log):
    while listen_log.is_set():
        time.sleep(0.01)
        if len(shared_list) > 0:
            print(shared_list.pop(0))
    
    while len(shared_list) > 0:
        print(shared_list.pop(0))


if __name__ == '__main__':
    # Create a shared list using Manager
    manager = multiprocessing.Manager()
    listen_log = multiprocessing.Event()
    shared_list = manager.list()

    # Define two processes to write to the shared list
    log_proc = multiprocessing.Process(target=write_logs, 
                                       args=(shared_list, listen_log))

    # Start process
    listen_log.set()
    log_proc.start()

    # Execute the service
    thread_1 = threading.Thread(target=threadfoo_1,
                                args=(shared_list, ))
    
    thread_2 = threading.Thread(target=threadfoo_2,
                                args=(shared_list, ))

    thread_1.start()
    thread_2.start()

    thread_1.join()
    thread_2.join()

    # Join process to wait for it to finish
    listen_log.clear()
    log_proc.join()