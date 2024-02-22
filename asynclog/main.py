from concurrent_logging import ConcurrentLogger
from concurrent_logging import concurrent_logging, log_queue


@concurrent_logging('%(asctime)s: %(message)s', log_queue)
def shout(clogger, msg):
    clogger.info(msg + '!')


if __name__ == '__main__':
    cl = ConcurrentLogger(False, True)
    cl.start()

    shout('Hello')

    cl.stop()