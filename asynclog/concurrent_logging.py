import multiprocessing
import logging
import time

from logging.handlers import RotatingFileHandler
from logging.handlers import QueueHandler
from logging.handlers import QueueListener


# This queue should be imported in every module where logging is to be done!
log_queue = multiprocessing.Queue()


class ConcurrentLogger:

    # Initialize the ConcurrentLogger object and choose output channels
    # (set the desired channel's value to True)
    def __init__(self, file_handler: bool, console_handler: bool):
        args = (log_queue, file_handler, console_handler)
        self._stop_event = multiprocessing.Event()
        self._process = multiprocessing.Process(target=self._worker,
                                                args=args)

    # Worker function, which handles log file/stream output
    def _worker(self, queue, file_handler, console_handler):
        handlers = []

        if file_handler: 
            handler = RotatingFileHandler(filename='log',
                                          mode='a',
                                          maxBytes=10_000,
                                          backupCount=10,
                                          encoding='utf-8')
            handlers.append(handler)

        if console_handler:
            handler = logging.StreamHandler()
            handlers.append(handler)

        listener = QueueListener(queue, *handlers)
        listener.start()

        while not self._stop_event.is_set():
            time.sleep(1)

        listener.stop()

    # Start the logger
    def start(self):
        self._process.start()

    # Stop the logger
    def stop(self):
        self._stop_event.set()
        self._process.join()    


# Decorator that enables logging for decorated function
def concurrent_logging(log_format, log_queue):
    def true_decorator(function):
        formatter = logging.Formatter(log_format)

        handler = QueueHandler(log_queue)
        handler.setFormatter(formatter)

        clogger = logging.getLogger(function.__module__ + function.__name__)
        clogger.setLevel(logging.DEBUG)
        clogger.addHandler(handler)

        def wrapper(*args, **kwargs):
            return function(clogger, *args, **kwargs)

        return wrapper
    return true_decorator