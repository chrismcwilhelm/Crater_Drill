from crater_can import WaveshareAdapter, CANFrame
import argparse
import threading

class CANManager:
    def __init__(self, port):
        self.bus = WaveshareAdapter(port)

        self._mailboxes = {}
        self._events = {}
        self.bus.listen(self.on_msg)

    def send_motor_command(self, can_id: int, motor_id: int, motor_command: int, direction: int = 0xFF, speed: int = 0xFF):

        data = [
            motor_id,
            motor_command,
            direction,
            speed,
        ]

        self.bus.send(can_id, data)

    def send_sample_command(self, can_id: int, command: int, speed: int = 0xFF):
        self.bus.send(can_id, [command, speed])

    def on_msg(self, frame: CANFrame) -> None:
        key = frame.id
        self._mailboxes[key] = frame.data

        if key in self._events:
            self._events[key].set()

    def request_and_wait(self, tx_id: int, payload: list[int], rx_id: int, timeout: float = 1.0):
        event = threading.Event()
        self._events[rx_id] = event
        self._mailboxes.pop(rx_id, None)

        self.bus.send(tx_id, payload)

        if not event.wait(timeout):
            self._events.pop(rx_id, None)
            return None

        self._events.pop(rx_id, None)
        return self._mailboxes.get(rx_id)