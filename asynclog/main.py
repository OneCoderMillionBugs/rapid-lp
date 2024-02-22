from concurrent_logging import ConcurrentLogger
from concurrent_logging import concurrent_logging, log_queue

from concurrent.futures import ProcessPoolExecutor
from concurrent.futures import wait


@concurrent_logging('%(asctime)s: %(message)s', log_queue)
def shout(clogger, pid):
    for i in range(1000):
        clogger.info(str(pid) + ': ' + str(i))


if __name__ == '__main__':
    cl = ConcurrentLogger(False, True)
    cl.start()

    futures = []

    with ProcessPoolExecutor(max_workers=4) as exec:
        for i in range(4):
            futures.append(exec.submit(shout(i)))

    wait(futures)
    cl.stop()